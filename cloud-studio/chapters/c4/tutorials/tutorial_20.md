# 条款 20：宁以 pass-by-reference-to-const 替换 pass-by-value

缺省情况下 C++ 以 by value 方式（继承 C）传递对象至函数。除非你另外指定，否则函数参数都是以实际实参的副本为初值，而调用段所获得的也是函数返回值的一个副本。这些副本是由对象的 copy 构造函数产出，这可能使得 pass-by-value 成为昂贵（费时）的操作。

## 传值的成本

参考下面的例子：

```cpp
Student plato;
bool platoIsOK = validateStudent(plato);  // 调用函数
```

当上述函数被调用时，参数的传递成本是"一次 Student copy 构造函数调用，加上一次 Student 析构函数调用"。而且 Student 对象内还会有其他的对象。尽管成功实现了客户的希望，但是却花费了很大的代价。

通过 pass by reference to const 的方式可以回避这些构造和析构动作：

```cpp
bool validateStudent(const Student& s);
```

这种传递方式效率高得多，没有任何构造函数或析构函数被调用，因为没有任何新对象被创建。修订后这个参数声明中的 `const` 是重要的，因为不这样做的话调用者可能会改变它们传入的那个 Student 对象。

## 避免对象切割

通过引用方式传递参数也可以避免 slicing（对象切割）问题。当一个 derived class 对象以 by value 方式传递并被视为一个 base class 对象，base class 的 copy 构造函数会被调用，而派生类的特化性质全被切割掉了，仅仅留下一个基类对象所拥有的性质。

参考下面的例子：

```cpp
class Window {
public:
    std::string name() const;
    virtual void display() const;
};

class WindowWithScrollBars : public Window {
    virtual void display() const;
};
```

需要注意的是 `display` 是个 virtual 函数，这意味着基类对象和派生类对象的显示方式是不同的（见条款 34 和 36）。假设你希望写个函数打印窗口名称，然后显式该窗口，下面是个错误的示范：

```cpp
void printNameAndDisplay(Window w) {  // 错误，参数可能被切割
    std::cout << w.name();
    w.display();
}
```
当调用上述函数并交给它一个 `WindowWithScrollBars` 对象时，参数 `w` 会被构造成一个 `Window`，并且丢弃 `WindowWithScrollBars` 对象所有特化信息，而二者的 `display` 是不一样的。

解决切割问题的办法就是，以 by reference-to-const 的方式传递给 w:

```cpp
void printNameAndDisplay(const Window& w) {  // 正确，参数不会被切割
    std::cout << w.name();
    w.display();
}
```

- 整体的例子

```cpp
class Window {

    Window(const std::string& title) : title_(title) {}
    
    // 虚析构函数
    virtual ~Window() = default;
    
    // 常规成员函数
    std::string name() const { return title_; }
    
    // 虚函数：不同窗口类型有不同的显示方式
    virtual void display() const {
        std::cout << "基类Window: " << title_ << std::endl;
    }
    
    // 虚函数：不同窗口类型有不同的尺寸
    virtual int getSize() const { return 100; }

private:
    std::string title_;
}

class WindowWithScrollBars : public Window {
public:
    // 构造函数，转发参数给基类
    WindowWithScrollBars(const std::string& title, int scrollPos = 0) 
        : Window(title), scrollPosition_(scrollPos) {}
    
    // 重写显示函数
    void display() const override {
        std::cout << "带滚动条的窗口: " << name() 
                  << ", 滚动位置: " << scrollPosition_ << std::endl;
    }
    
    // 重写尺寸计算
    int getSize() const override {
        // 基类尺寸加上滚动条的尺寸
        return Window::getSize() + 20;
    }
    
    // 滚动条特有的功能
    void scroll(int newPos) {
        scrollPosition_ = newPos;
    }

private:
    int scrollPosition_;  // 派生类特有的状态
};

// 演示对象切割的函数
void demonstrateSlicing() {
    WindowWithScrollBars scrollWindow("My Scroll Window", 50);
    
    std::cout << "\n=== 通过引用调用（没有对象切割）===" << std::endl;
    const Window& windowRef = scrollWindow;
    windowRef.display();  // 调用派生类版本
    std::cout << "窗口尺寸: " << windowRef.getSize() << std::endl;
    
    std::cout << "\n=== 通过值传递（发生对象切割）===" << std::endl;
    Window windowVal = scrollWindow;  // 对象切割发生在这里！
    windowVal.display();  // 调用基类版本
    std::cout << "窗口尺寸: " << windowVal.getSize() << std::endl;
    
    // 原始对象保持不变
    std::cout << "\n=== 原始对象保持完整 ===" << std::endl;
    scrollWindow.display();
    scrollWindow.scroll(100);  // 仍然可以调用派生类特有的方法
    scrollWindow.display();
}

## 内置类型的传递

如果窥视 C++ 编译器的底层，你会发现，reference 往往以指针实现出来，因此 pass by reference 通常意味真正传递的是指针。因此如果你有个对象属于内置类型，pass by value 往往比 pass by reference 的效率高些。这个忠告也适用于 STL 的迭代器和函数对象，因为习惯上它们都被设计为 passed by value。迭代器和函数对象的实践者有责任看看它们是否高效且不受切割问题的影响。这取决于你使用哪一部分 C++（见条款 1）。

一般而言可以理解 pass by value 的唯一对象就是内置类型和 STL 的迭代器和函数对象，至于其他任何东西，尽量使用 pass-by-reference-to-const。

## 请记住

- 尽量以 pass-by-reference-to-const 替换 pass-by-value。前者通常比较高效，并可以避免切割问题。
- 以上规则并不适用于内置类型，以及 STL 的迭代器和函数对象。对它们而言，pass-by-value 往往比较适当。