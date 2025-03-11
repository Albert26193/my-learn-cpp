# 05 C++ 默认生成的特殊成员函数

## 编译器自动生成的函数

当我们定义一个空类时：

```cpp
class Empty {};
```

编译器会默认生成以下特殊成员函数，相当于我们自己写了如下代码：

```cpp
class Empty {
public:
    Empty() {}                         // 默认构造函数
    Empty(const Empty& rhs) {}         // 拷贝构造函数，复制所有非静态成员
    Empty& operator=(const Empty& rhs) { // 拷贝赋值运算符
        return *this;
    }
    ~Empty() {}                        // 析构函数
};
```

## 自动生成的条件

编译器生成这些函数是有条件的：

1. 如果我们显式定义了对应函数，编译器将不再生成其默认实现
2. 这些函数只有在被调用时才会被编译器创建

## 实验：验证自动生成的函数

我们可以通过查看反汇编文件来确认 C++ 自动生成了哪些函数：

```bash
# 生成汇编文件
clang++ -S -std=c++11 -Wextra tutorial_05.cpp

# 使用 c++filt 解析符号名
c++filt <tutorial_05.s中的符号名>
```

## 拒绝自动生成拷贝赋值函数的场景

以下情况下编译器会拒绝生成拷贝赋值函数：

### 1. 违反语言规则
- 修改 `const` 成员的值
- 修改引用成员指向
- 基类将拷贝赋值运算符声明为私有

### 2. 示例代码

```cpp
class A {
public:
    A(int a): a(a) {}
    ~A() {}

private:
    A& operator=(const A& rhs) {  // 私有的拷贝赋值运算符
        this->a = rhs.a;
        return *this;
    }
    int a;
};

class B: public A {               // 派生类
public:
    B(int a): A(a) {}
    ~B() {}
};
```

## 总结

编译器自动生成的特殊成员函数是 C++ 语言的一个重要特性，但在某些情况下编译器会拒绝生成这些函数。了解这些规则和限制有助于我们更好地使用 C++ 语言。
