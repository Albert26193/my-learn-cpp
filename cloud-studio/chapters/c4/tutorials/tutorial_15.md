# 条款15：在资源管理类中提供对原始资源的访问

## 问题背景

资源管理类通常封装了对原始资源（如指针）的访问，但有时候我们需要直接使用这些原始资源。一个设计良好的资源管理类应该提供对原始资源的访问方法，如 `get()` 成员函数或重载指针操作符。

## 错误示例：无法访问原始资源

```cpp
class BadCase {
public:
    BadCase() : ptr_(new int(0)) {}
    ~BadCase() { delete ptr_; }

private:
    int* ptr_;
};

int main() {
    // 无法访问原始资源（int*），因此无法对其进行操作
    BadCase w;
    // 无法使用 w.ptr_ 或获取指针值
    return 0;
}
```

## 正确示例：提供对原始资源的访问

```cpp
class GoodCase {
public:
    GoodCase() : ptr_(new int(0)) {}
    ~GoodCase() { delete ptr_; }

    // 提供对原始资源的访问
    int* get() const { return ptr_; }

private:
    int* ptr_;
};

int main() {
    GoodCase w;
    // 现在可以访问原始资源（int*）并对其进行操作
    *w.get() = 42;
    std::cout << "Wrapped int value: " << *w.get() << std::endl;
    return 0;
}
```

## 其他访问原始资源的方法

除了 `get()` 方法外，还可以提供其他方式访问原始资源：

1. **重载指针操作符**：允许资源管理类像指针一样使用
2. **隐式转换**：提供到原始资源类型的隐式转换（但要小心使用）

## 代码运行命令

```bash
clang++ tutorial15.cpp -o app && ./app