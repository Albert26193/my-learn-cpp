# C++ 中 = delete 的高级用法

## 1. 禁用隐式类型转换

```cpp
class Number {
public:
    // 只允许显式的整数构造
    explicit Number(int x) : value(x) {}
    
    // 禁用浮点数构造，防止精度丢失
    Number(double) = delete;
    Number(float) = delete;
    
private:
    int value;
};

int main() {
    Number n1(42);        // 正确：显式整数构造
    Number n2(3.14);      // 错误：double 构造被禁用
    Number n3 = 42;       // 错误：explicit 禁止隐式转换
}
```

## 2. 禁用特定的函数重载

```cpp
class Logger {
public:
    // 只允许使用 std::string 作为参数
    void log(const std::string& message);
    
    // 禁用不安全的 C 风格字符串
    void log(const char*) = delete;
    
    // 禁用其他类型，防止意外的类型转换
    void log(void*) = delete;
};
```

## 3. 禁用动态内存分配

```cpp
class StackOnly {
public:
    StackOnly() = default;
    
    // 禁用 new 运算符
    static void* operator new(std::size_t) = delete;
    static void* operator new[](std::size_t) = delete;
    
    // 禁用 delete 运算符
    static void operator delete(void*) = delete;
    static void operator delete[](void*) = delete;
};
```

## 4. 限制函数调用方式

```cpp
class ThreadSafe {
public:
    // 只允许在堆上创建对象
    static ThreadSafe* create() {
        return new ThreadSafe();
    }
    
    // 禁用栈对象
    ~ThreadSafe() = delete;
    
private:
    ThreadSafe() = default;
};
```

## 5. 防止模板特化中的某些类型

```cpp
template<typename T>
class Container {
public:
    void store(const T& value);
};

// 禁用 void* 特化
template<>
void Container<void*>::store(void* const&) = delete;
```

## 6. 禁用某些运算符

```cpp
class NoCompare {
public:
    // 禁用相等比较
    bool operator==(const NoCompare&) const = delete;
    bool operator!=(const NoCompare&) const = delete;
    
    // 禁用小于比较
    bool operator<(const NoCompare&) const = delete;
};
```

## 最佳实践

1. **明确的意图**：
   - 使用 `= delete` 比将函数声明为 private 更清晰
   - 编译器会提供更好的错误信息

2. **安全性考虑**：
   - 禁用可能导致问题的隐式转换
   - 禁用不安全的操作（如 C 风格字符串）
   - 防止资源泄漏

3. **设计考虑**：
   - 在类的公有部分声明 deleted 函数
   - 配合 `explicit` 关键字使用
   - 考虑是否需要同时删除相关的函数（如同时删除拷贝构造和赋值运算符）

4. **调试友好**：
   - deleted 函数会在编译时报错
   - 错误信息清晰指出问题所在
   - 有助于捕获设计违例
