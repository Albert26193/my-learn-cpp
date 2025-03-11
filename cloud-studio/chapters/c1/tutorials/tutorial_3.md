# 条款03：尽可能使用 const

## const 语义

`const` 关键字用于声明不该被改动的对象。使用 `const` 可以让编译器强制实施这种约束，从而防止程序中的意外修改。

## const 讲解

### 指针与 const

```cpp
char greeting[] = "Hello";
char* p = greeting;                // 非常量指针，非常量数据
const char* p = greeting;          // 非常量指针，常量数据
char* const p = greeting;          // 常量指针，非常量数据 
const char* const p = greeting;    // 常量指针，常量数据
```

### const 在 * 左边

当 `const` 在星号左边时，表示被指物是常量：

```cpp
int a = 10;
int b = 20;
const int *ptr = &a;
*ptr = 30;  // 编译错误：不能通过ptr修改a的值
ptr = &b;   // 允许：ptr现在指向b
```

### const 在 * 右边

当 `const` 在星号右边时，表示指针自身是常量：

```cpp
int a = 10;
int b = 20;
int *const ptr = &a;
*ptr = 30;  // 允许：通过ptr修改a的值
ptr = &b;   // 编译错误：不能改变ptr的值
```

### const 在 * 两边

当 `const` 在星号两边时，表示被指物和指针两者都是常量：

```cpp
int a = 10;
int b = 20;
const int *const ptr = &a;
*ptr = 30;  // 编译错误：不能通过ptr修改a的值
ptr = &b;   // 编译错误：不能改变ptr的值
```

> **提示**：`const` 语法虽然变化多端，但并不莫测高深。如果关键字 `const` 出现在星号左边，表示被指物是常量；如果出现在星号右边，表示指针自身是常量；如果出现在星号两边，表示被指物和指针两者都是常量。

## const 与迭代器

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5};

// 相当于 const T *it1;
std::vector<int>::const_iterator it1 = numbers.begin();
*it1 = 10;  // 报错
++it1;      // 正确

// 相当于 T* const it2;
std::vector<int>::iterator const it2 = numbers.begin();
*it2 = 10;  // 正确
++it2;      // 报错
```

## const 作为返回值

```cpp
class Number {
public:
    int value_;
    Number(int value) : value_(value) {}

    Number operator*(const Number &other) const {
        return Number(value_ * other.value_);
    }

    const Number operator*(const Number &other) const {
        return Number(value_ * other.value_);
    }
};

int main() {
    Number num1(3);
    Number num2(4);
    Number num3(1);

    const Number result = num1 * num2;  // 可运行
    (num1 * num2) = num3;              // 可运行，因为可以作为左值
    
    return 0;
}
```

## const 其他用法

```cpp
#include <iostream>
#include <string>

// const 用于全局变量
const int global_const = 42;

class MyClass {
public:
    // const 用于成员变量
    const std::string name;

    // 构造函数参数使用 const
    MyClass(const std::string &name) : name(name) {}

    // const 用于成员函数本体
    void PrintName() const {
        std::cout << "Name: " << name << std::endl;
    }

    // const 用于函数返回类型
    const int GetValue() const {
        return value_;
    }

private:
    int value_ = 10;
};

// const 用于函数参数
void PrintValue(const int value) {
    std::cout << "Value: " << value << std::endl;
}

int main() {
    // const 用于局部变量
    const int local_const = 100;

    MyClass obj("Alice");

    // 输出全局常量、局部常量、成员变量、函数返回值
    std::cout << "Global const: " << global_const << std::endl;
    std::cout << "Local const: " << local_const << std::endl;
    std::cout << "Member const: " << obj.name << std::endl;
    std::cout << "Const return value: " << obj.GetValue() << std::endl;

    // 调用带有 const 参数的函数
    PrintValue(local_const);

    // 调用 const 成员函数
    obj.PrintName();

    return 0;
}
```

### const 用法说明

- **全局变量**：`global_const` 被声明为一个常量整数，其值不能被修改。
- **局部变量**：`local_const` 被声明为一个常量整数，其值不能被修改。
- **函数参数**：`PrintValue()` 函数的参数被声明为 `const int`，这意味着在函数内部不能修改参数的值。
- **函数返回类型**：`MyClass` 类中的 `GetValue()` 函数返回一个常量整数，这意味着我们不能对返回值进行修改。
- **成员函数本体**：`MyClass` 类中的 `PrintName()` 函数被声明为 `const` 成员函数，这意味着在函数内部不能修改类的任何成员变量（除非它们是 `mutable`）。
- **成员变量**：`MyClass` 类中的 `name` 成员变量被声明为 `const std::string`，这意味着它的值在对象创建后不能被修改。

## 代码运行命令

```bash
clang++ tutorial3.cpp -o app && ./app
```