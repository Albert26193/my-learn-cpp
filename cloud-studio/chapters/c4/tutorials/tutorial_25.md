# 条款 25：考虑写出一个不抛异常的 swap 函数

## 标准库中的 swap 实现

所谓 swap，就是将两个对象的值置换。std 中提供了 swap 的典型实现如下：

```cpp
namespace std {
    template<typename T>
    void swap(T& a, T& b) {
        T temp(a);
        a = b;
        b = temp;
    }
}
```

只要类型 `T` 支持 copying（即实现了拷贝构造函数和拷贝赋值操作符），则 `std::swap` 就能正常工作。

## 效率问题：以 pimpl 模式为例

但是，能正常工作不代表效率高。比如当需要 swap 一个用 pimpl（pointer to implementation）手法设计的类时：

```cpp
class WidgetImpl {
public:
    // ...
private:
    int a, b, c;
    std::vector<double> v;     // vector 很大时，深拷贝效率低
};

class Widget {                 // 该类使用了 pimpl 手法
public:
    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs) {
        // ...
        *pImpl = *(rhs.pImpl); // 关于 operator= 的详细讨论参见条款 10~12
        // ...
    }
    // ...
private:
    WidgetImpl* pImpl;
};
```

对于 `Widget` 类而言，要置换其两个对象，实际只需要交换两个对象的 `pImpl` 指针即可。但 `std::swap` 并不知道这一点，它还是会严格执行深复制，不止复制了 3 个 `Widget` 对象，还复制了其包含的 3 个 `WidgetImpl` 对象。

## 优化方案

要解决这个问题，与 STL 容器一致的做法是：
1. 为 `Widget` 类提供一个 `public swap` 成员函数
2. 基于此实现一个 **std::swap 的特化版本**

## 模板类的特殊情况

上述解决办法在处理类时可以工作得很好，但是当 `Widget` 和 `WidgetImpl` 作为类模板实现时，就会出现新的问题：

- `public swap` 成员函数的实现照旧，不会有任何问题
- 但 `std::swap` 的特化版本将会遇到问题：
  1. 不能使用偏特化（函数模板不支持偏特化）
  2. 不能在 `std` 命名空间中添加新的函数模板（重载）

## 解决方案

答案很简单：将这个 swap 函数模板作为一个 non-member 函数模板置于 Widget 类模板所在的命名空间（置于 global 命名空间也合法，但不优雅）。

这种实现我们姑且称之为 swap 之于 Widget 的专属版本。这种实现其实对于类和类模板都是适用的。

## 最佳实践建议

为了尽可能地让你的 swap 优化版本适用于各种情况，建议：
1. 在类所在的命名空间实现一个 non-member 专属版本
2. 同时实现一个 `std::swap` 的特化版本

## 调用方式比较

### 推荐的调用方式
```cpp
template<typename T>
void doSomething(T& obj1, T& obj2) {
    using std::swap;  // 使 std::swap 可见
    swap(obj1, obj2); // 编译器会选择最合适的版本
}
```

这种调用方式的好处是，编译器会基于类型去匹配最合适的调用：
- 如果 `T` 是 `Widget` 类型，且其命名空间中存在专属版本的 swap，则会调用它
- 若不存在，则由于 `using std::swap` 的存在，编译器还可以用 `std::swap` 作为保底方案
- 若 `std::swap` 的特化版本存在，则会选择特化版本

### 不推荐的调用方式
```cpp
std::swap(obj1, obj2);  // 限制了只能使用 std::swap
```

这种调用方式不推荐，因为这限制编译器只能选择 `std::swap` 及其特化版本。这时实现一个 `std::swap` 的特化版本的作用就体现出来了：即便用户使用了不被推荐的调用方式，也能够让你的 swap 优化版本得以被调用。

## 总结

1. 如果缺省版本的 `std::swap` 能够提供可接受的效率，则你只需要定义好你的对象的拷贝动作即可

2. 如果缺省版本的效率不足（几乎总是意味着你的 class 或 class template 使用了某种 pimpl 手法），则可以考虑如下：
   - 提供一个 `public swap` 成员函数，能够高效地处理置换操作，且该函数绝不该抛出异常
   - 在类或类模板所在的命名空间中提供一个 non-member swap 函数，并令其调用上述成员函数
   - 如果你在实现一个类（而非类模板），则为你的类特化 `std::swap` 函数，令其调用上述成员函数

3. 最后，在调用时：
   - 请包含一个 `using` 声明式，让 `std::swap` 可用
   - 在调用 swap 时不加任何 namespace 修饰符