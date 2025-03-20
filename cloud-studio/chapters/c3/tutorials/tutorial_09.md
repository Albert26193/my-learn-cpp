# 09 不在构造析构过程中调用virtual函数

## 问题描述

当派生类被实例化时，由于派生类中的基类成员变量先被构造，因此是先调用`base`的构造函数，然后再调用`deriveA`的构造函数，也就是说，`base`版本的`foo`函数会先于`deriveA`版本的`foo`函数被调用，达不到预期目的。

相同道理适用于析构函数。一旦派生类析构函数开始执行，派生类的成员变量便呈现未定义值，再进入基类析构函数后，对象就“退化”为一个基类对象，调用的`virtual`函数又是`base`版本的`foo`函数，达不到预期目的。

对于上述情况，某些编译器会发出警告。

```cpp
#include <cstdio>

class Base {
public:
    Base() {
        printf("base constructor\n");
        foo();
    }
    virtual ~Base() {
        printf("base destructor \n");
        foo();
    }
    virtual void foo() const {
        printf("base foo \n");
    }
};

class Derived : public Base {
public:
    Derived(){
        printf("derived constructor \n");
        foo();
    }
    ~Derived() {
        printf("derived destructor \n");
        foo();
    }
    void foo() const override {
        printf("derived foo \n");
    }
};

int main() {
    Derived* ptr = new Derived;
    cout << endl;
    delete ptr;
    return 0;
}
```

## 讨论

拥有虚函数派生类的构造析构顺序：
“洋葱模型”

`constructor` 和 `destructor` 过程中是否能呈现多态性？
不能，所以不要在构造析构过程中调用`virtual`函数

怎么样让 `Derived` 在`destructor` 中调用 `base::foo` 

```bash
clang++ tutorial9.cpp -o app && ./app
```

##  分析

- 上面例子的调用顺序 (穿脱原理)
  1. `Base`的构造函数
  2. `Derived`的构造函数
  3. `Derived`的析构函数
  4. `Base`的析构函数
