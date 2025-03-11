# 06 禁用编译器自动生成的函数

## 问题引入：银行卡复制的案例

在银行业务系统中，如果 `BankCard` 类中存储了余额信息，允许卡片之间的复制操作将会带来严重的问题：

```cpp
class BankCard {
private:
    uint64_t balance;
};

int main() {
    BankCard card1;
    BankCard card2;
    BankCard card3(card1); // 危险：通过拷贝构造函数复制 card1
    card1 = card2;         // 危险：通过赋值运算符复制 card2
    return 0;
}
```

编译器会默认生成拷贝构造函数和拷贝赋值运算符，这可能导致余额被意外复制。

## 禁用复制的方法

### 1. private 声明法（不推荐）

将拷贝相关函数声明为 private 是一种方法，但存在以下问题：
- 成员函数和友元函数仍然可以访问 private 函数
- 只有在链接时才会发现错误（linkage error）

示例：
```cpp
class B;

class A {
    friend B;
public:
    A(int a): a(a) {}
    ~A() {}

private:
    A& operator=(const A& rhs) {
        this->a = rhs.a;
        return *this;
    }
    int a;
};

class B {
public:
    B(int a, int b): a(a), b(b) {}
    ~B() {}
    void assign() {
        a = b;  // 合法操作，因为 B 是 A 的友元
    }
private:
    A a;
    A b;
};
```

### 2. Uncopyable 基类方法（推荐）

更好的做法是设计一个专门用来阻止复制的基类。这样可以：
- 在编译期就发现问题
- 提供更清晰的错误信息
- IDE 可以及时提示错误

```cpp
class Uncopyable {
protected:
    Uncopyable() {}                              // 允许构造和析构
    ~Uncopyable() {}

private:
    Uncopyable(const Uncopyable&);              // 禁止复制
    Uncopyable& operator=(const Uncopyable&);    // 禁止赋值
};

class BankCard: private Uncopyable {            // 私有继承
public:
    BankCard() {}
private:
    uint64_t balance;
};
```

## 编译和运行

```bash
clang++ -std=c++11 -Wall -Wextra tutorial_06.cpp -o app && ./app
```

## 最佳实践

1. 如果类的语义不允许复制，应该明确禁止
2. 使用基类继承的方式比声明为 private 更好
3. 在 C++11 之后，也可以使用 `= delete` 来达到同样的效果