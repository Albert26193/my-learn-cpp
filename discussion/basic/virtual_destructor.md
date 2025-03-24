# C++中的虚析构函数

## 什么是析构函数？

析构函数是C++中的特殊成员函数，当对象生命周期结束时被自动调用。它负责执行清理工作，如释放资源、关闭文件和释放动态分配的内存。析构函数的名称是类名前加上波浪号`~`，例如：`~ClassName()`。

析构函数的主要特点：
- 不接受任何参数
- 没有返回类型
- 一个类只能有一个析构函数
- 如果没有显式定义，编译器会提供一个默认析构函数

```cpp
class Example {
public:
    // 构造函数
    Example() { 
        data = new int[100]; // 分配资源
    }
    
    // 析构函数
    ~Example() { 
        delete[] data; // 释放资源
    }
    
private:
    int* data;
};
```

## 为什么基类需要虚析构函数？

在C++中，当通过基类指针删除派生类对象时，如果基类的析构函数不是虚函数，则只会调用基类的析构函数，而不会调用派生类的析构函数。这会导致以下问题：

### 1. 内存泄漏

派生类可能会分配其他资源，如果派生类的析构函数不被调用，这些资源就无法释放，导致内存泄漏。

```cpp
class Base {
public:
    ~Base() { /* 基类清理 */ }
};

class Derived : public Base {
public:
    Derived() { ptr = new int[10]; }
    ~Derived() { delete[] ptr; } // 不会被调用!
private:
    int* ptr;
};

int main() {
    Base* b = new Derived();
    delete b; // 只调用Base::~Base()，不调用Derived::~Derived()
              // 导致ptr所指的内存泄漏
    return 0;
}
```

### 2. 对象不完全析构

派生类对象的派生部分实际上没有被正确析构，可能导致程序状态不一致或其他难以追踪的bug。

### 3. 影响继承体系的可扩展性

不使用虚析构函数会限制基类的设计意图，使其难以作为多态基类使用。

## 如何解决？使用虚析构函数

将基类的析构函数声明为虚函数，确保通过基类指针删除对象时，会调用最派生类的析构函数，然后依次调用所有基类的析构函数。

```cpp
class Base {
public:
    virtual ~Base() { /* 基类清理 */ }
};

class Derived : public Base {
public:
    Derived() { ptr = new int[10]; }
    ~Derived() override { delete[] ptr; } // 现在会被正确调用
private:
    int* ptr;
};

int main() {
    Base* b = new Derived();
    delete b; // 首先调用Derived::~Derived()，然后调用Base::~Base()
    return 0;
}
```

## 实际案例分析

在我们的项目中，`BaseNode`类没有将析构函数声明为虚函数，导致以下问题：

```cpp
TEST_F(NodeTest, IsVitrualDestructor) {
  auto node1 = create_node(PageNodeType);
  auto node2 = create_node(RectangleNodeType);
  delete node1;
  delete node2;
  EXPECT_EQ(BaseNode::get_count(), 0);
}
```

这个测试失败，因为：

1. `node1`和`node2`是通过`BaseNode*`指针管理的派生类对象
2. 删除这些对象时，只调用了`BaseNode`的析构函数
3. 派生类`PageNode`和`RectangleNode`的析构函数没有执行，导致：
   - `dec_count()`没有被调用，计数器没有减少
   - `RectangleNode`分配的`width`和`height`资源没有释放

## 最佳实践

1. **规则：如果一个类设计用于被继承，其析构函数应该是虚函数**
2. 如果类不打算被继承，可以标记为`final`并使用非虚析构函数
3. 在派生类中，使用`override`关键字明确表示覆盖基类的虚函数
4. 考虑为基类提供`protected`构造函数，防止错误地直接实例化

## 总结

虚析构函数是C++多态机制的重要组成部分。为了确保对象的正确析构和资源的释放，任何可能成为基类的类都应该有虚析构函数。这不仅可以防止内存泄漏，还能确保代码的健壮性和可维护性。
