# noexcept 关键字详解

## 设计目的

noexcept 是 C++11 引入的关键字，用于：
1. 向编译器声明函数不会抛出异常
2. 提供更好的优化机会
3. 提供更强的异常安全保证
4. 在移动语义中尤其重要

## 语法与用法

1. 基本声明：
```cpp
void func() noexcept;              // 保证不抛出异常
void func() noexcept(true);       // 等价形式
void func() noexcept(condition);   // 条件形式
```

2. 条件形式示例：
```cpp
template<class T>
void swap(T& a, T& b) noexcept(noexcept(T(std::move(a)))) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}
```

## 重要应用场景

1. 移动操作：
```cpp
class String {
public:
    // 移动构造函数应该是 noexcept 的
    String(String&& other) noexcept;
    // 移动赋值运算符也应该是 noexcept 的
    String& operator=(String&& other) noexcept;
};
```

2. swap 函数：
```cpp
void swap(Widget& a, Widget& b) noexcept {
    // 只交换指针，不会抛出异常
    std::swap(a.pImpl, b.pImpl);
}
```

## 编译器优化

1. 性能提升：
- 编译器可以省略异常处理代码
- 可以避免生成栈展开表
- 允许更激进的优化

2. 容器操作优化：
- vector 在扩容时会优先使用移动而不是拷贝
- 但前提是移动操作被标记为 noexcept

## 注意事项

1. 违反承诺的后果：
- 如果 noexcept 函数抛出异常，程序会直接终止
- 不会进行栈展开
- 不会调用析构函数

2. 使用建议：
- 不要轻易承诺 noexcept
- 确保函数真的不会抛出异常
- 优先用于移动操作和 swap 函数
- 考虑使用条件形式而不是无条件形式

## 最佳实践

1. 应该使用 noexcept 的场合：
- 移动构造函数
- 移动赋值运算符
- swap 函数
- 析构函数（默认就是 noexcept）
- 内存释放函数
- 简单的 getter/setter

2. 不应该使用 noexcept 的场合：
- 可能分配内存的函数
- 可能抛出异常的系统调用
- 复杂的业务逻辑函数
- 依赖外部状态的函数
