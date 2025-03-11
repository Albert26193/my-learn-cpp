# C++中的`const`关键字详解

## 目录

- [基本概念](#基本概念)
- [指针与`const`](#指针与-const)
- [函数参数中的`const`](#函数参数中的-const)
- [成员函数中的`const`](#成员函数中的-const)
- [返回值中的`const`](#返回值中的-const)
- [运算符重载中的`const`](#运算符重载中的-const)
- [常见错误与陷阱](#常见错误与陷阱)
- [最佳实践](#最佳实践)

## 基本概念

`const`关键字用于声明不应该被修改的对象。它是C++语言中实现"常量性"的主要机制，可以应用于变量、指针、引用、成员函数和返回值等多种场景。

```cpp
// 基本用法
const int x = 10;  // x是一个常量整数，不能被修改
x = 20;  // 编译错误
```

## 指针与`const`

`const`与指针结合使用时，可以有不同的含义，取决于`const`关键字的位置。

### `const`在星号左边

当`const`在星号左边时，表示被指物是常量（指针指向的内容不能被修改）。

```cpp
int a = 10;
int b = 20;
const int* ptr = &a;  // 指向常量的指针
*ptr = 30;  // 编译错误：不能通过ptr修改a的值
ptr = &b;   // 允许：ptr现在指向b
```

### `const`在星号右边

当`const`在星号右边时，表示指针自身是常量（指针的指向不能被修改）。

```cpp
int a = 10;
int b = 20;
int* const ptr = &a;  // 常量指针
*ptr = 30;  // 允许：通过ptr修改a的值
ptr = &b;   // 编译错误：不能改变ptr的指向
```

### `const`在星号两边

当`const`在星号两边时，表示被指物和指针两者都是常量。

```cpp
int a = 10;
int b = 20;
const int* const ptr = &a;  // 指向常量的常量指针
*ptr = 30;  // 编译错误：不能通过ptr修改a的值
ptr = &b;   // 编译错误：不能改变ptr的指向
```

## 函数参数中的`const`

在函数参数中使用`const`可以防止函数意外修改传入的参数。这在使用引用传递时特别重要。

```cpp
// 使用const参数
void printValue(const int& value) {
    // value = 100;  // 编译错误！
    std::cout << value << std::endl;
}

// 在类的成员函数中
class Number {
public:
    // 参数中的const表示不会修改传入的other对象
    auto operator*(const Number& other) const -> const Number {
        return Number(value_ * other.value_);
    }

private:
    int value_;
};
```

## 成员函数中的`const`

在成员函数声明的末尾添加`const`关键字，表示该函数不会修改对象的状态（即不会修改类的非`mutable`成员变量）。

```cpp
class Number {
public:
    int value_;
    
    Number(int value) : value_(value) {}
    
    // 非const成员函数 - 可以修改成员变量
    void increment() {
        value_++;  // 合法：可以修改value_
    }
    
    // const成员函数 - 不能修改成员变量
    void tryToIncrement() const {
        // value_++;  // 编译错误！不能在const成员函数中修改成员变量
    }
    
    // const成员函数 - 只能读取成员变量
    int getValue() const {
        return value_;  // 合法：可以读取value_
    }
};
```

### 使用场景

```cpp
int main() {
    Number num(5);         // 非const对象
    const Number constNum(10);  // const对象
    
    // 非const对象可以调用所有成员函数
    num.increment();       // 合法
    num.getValue();        // 合法
    num.tryToIncrement();  // 合法
    
    // const对象只能调用const成员函数
    // constNum.increment();       // 编译错误！const对象不能调用非const成员函数
    constNum.getValue();        // 合法
    constNum.tryToIncrement();  // 合法
}
```

## 返回值中的`const`

在函数返回值中使用`const`可以防止返回的对象被修改。这在运算符重载中尤其重要。

```cpp
class Number {
public:
    int value_;
    Number(int value) : value_(value) {}
    
    // 返回非const对象
    Number operator+(const Number& other) const {
        return Number(value_ + other.value_);
    }
    
    // 返回const对象
    const Number operator*(const Number& other) const {
        return Number(value_ * other.value_);
    }
};

int main() {
    Number a(5), b(10), c(15);
    
    (a + b) = c;  // 合法，因为+运算符返回非const对象
    // (a * b) = c;  // 编译错误，因为*运算符返回const对象
    
    return 0;
}
```

## 运算符重载中的`const`

在运算符重载中，`const`关键字可以出现在三个位置：

```cpp
auto operator*(const Number& other) const -> const Number;
//           ^                    ^      ^  
//           |                    |      |
//           |                    |      第三个位置：返回值的const
//           |                    第二个位置：成员函数的const
//           第一个位置：参数的const
```

1. **参数中的const**：表示函数不会修改传入的参数对象
2. **函数声明末尾的const**：表示函数不会修改调用对象（`this`指向的对象）
3. **返回值中的const**：表示返回的对象是常量，不能被修改

### 完整示例

```cpp
#include <iostream>
using namespace std;

class Number {
public:
  int value_;
  Number(int value) : value_(value) {
    cout << "alloc value: " << value << endl;
  }

  // 非const版本，返回非const对象
  auto operator*(const Number &other) -> Number {
    cout << "operation without const" << endl;
    return {value_ * other.value_};
  }

  // const版本，返回const对象
  auto operator*(const Number &other) const -> const Number {
    cout << "operation with const" << endl;
    return Number(value_ * other.value_);
  }

  // 非const成员函数 - 可以修改成员变量
  void increment() {
    value_++; // 合法：可以修改value_
  }

  // const成员函数 - 不能修改成员变量
  void tryToIncrement() const {
    // value_++;  // 编译错误！不能在const成员函数中修改成员变量
  }

  // const成员函数 - 只能读取成员变量
  [[nodiscard]] auto getValue() const -> int {
    return value_; // 合法：可以读取value_
  }
};

auto main() -> int {
  Number num1(3);
  Number num2(4);
  Number num3(1);

  const Number result = num1 * num2; // 调用非const版本
  (num1 * num2) = num3;              // 可以作为左值，因为返回非const对象

  const Number num4(4);
  const Number num5(1);
  num4 * num5; // 调用const版本，因为num4是const对象

  return 0;
}
```

## 常见错误与陷阱

1. **将非const引用绑定到临时对象**

```cpp
// 错误示例
string& ref = string("hello");  // 编译错误！不能将非const引用绑定到临时对象

// 正确的方式
const string& ref = string("hello");  // 正确，使用const引用
```

2. **在const成员函数中修改成员变量**

```cpp
class MyClass {
    int x_;
public:
    void badFunc() const {
        x_ = 10;  // 编译错误！不能在const成员函数中修改成员变量
    }
};
```

3. **返回局部变量的引用**

```cpp
// 错误示例
const string& getLocalString() {
    string local = "danger";
    return local;  // 错误！返回局部变量的引用
}
```

## 最佳实践

1. **尽可能使用`const`**

   - 对于不需要修改的变量，始终使用`const`声明
   - 函数参数尽量使用`const`引用或`const`指针
   - 不修改对象状态的成员函数应声明为`const`

2. **算术运算符重载应返回`const`对象**

   - 对于`+`、`-`、`*`、`/`等运算符，返回值应该是`const`的
   - 这可以防止如`(a + b) = c`这样的无意义操作

3. **使用`mutable`关键字处理特殊情况**

   - 当需要在`const`成员函数中修改成员变量时，可以将其声明为`mutable`
   - 这通常用于缓存、计数器等不影响对象逻辑状态的成员

4. **使用现代C++的语法特性**

   - 使用`auto`和尾后返回类型来提高代码可读性
   ```cpp
   auto getValue() const -> int { return value_; }
   ```
   
   - 使用`[[nodiscard]]`注解来强调返回值不应被忽略
   ```cpp
   [[nodiscard]] auto getValue() const -> int { return value_; }
   ```
