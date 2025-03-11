#include <iostream>
#include <vector>
using namespace std;

class B;

class A {
  // friend B;
  // private:
  public:
  A(int a)
    : a(a) {}
  ~A() = default;

  private:
  auto operator=(const A& rhs) -> A& = default;
  int  a;
};

class B : public A {
  public:
  B(int a)
    : A(a) {}
  ~B() = default;
  auto operator=(const B& rhs) -> B& { return *this; }
};


class Test {
  public:
  Test() {
    for (int i = 0; i < 10; i++) {
      b.emplace_back();
    }
  }
  ~Test() = default;
  [[nodiscard]] auto get_a() -> int const { return a; }

  [[nodiscard]] auto get_b() -> std::vector<int> const { return b; }

  private:
  int              a{};
  std::vector<int> b;
};


class Test2 {
  public:
  Test2()  = default;
  ~Test2() = default;
  /**
    第一个 const（即成员函数后面的 const）表示这是一个常量成员函数（const member
    function）。这意味着该函数承诺不会修改调用对象（即 Test2 类的实例）的任何成员变量。

    1. 这个 const 修饰的是 this 指针，使其变为 const Test2* const this
    2. 在这个函数内部，你不能修改任何非 mutable 的成员变量
    3. 这种设计是一种很好的实践，因为它向调用者承诺了函数的"只读"特性，有助于代码的安全性和可维护性
   */
  [[nodiscard]] auto get_a() const -> const Test& { return a; }

  private:
  Test a;
};

class Empty {
  public:
  // Empty(int c): c(c){}
  void print() {
    // std::cout << a.get_a().get_b()[0] << std::endl;
    std::cout << b << std::endl;
  }
  void set_b(int b) const { this->b = b; }

  private:
  // Test2 a;
  mutable int b;
  // const int c;
};


auto main() -> int {
  // 默认构造 _ZN5EmptyC2Ev
  // 没有显式的构造函数，但是实际上是发生了构造动作的
  Empty e1{};
  // copy 构造 _ZN5EmptyC2ERKS_
  // copy constructor 同样没有显式的声明，但是也是发生的
  Empty e2(e1);
  Empty e3(e1);
  Empty e4(e1);
  Empty e5(e1);
  // 赋值构造 _ZN5EmptyaSEOS_
  e3.set_b(5);
  e2 = e3;
  e2.print();
  // e1.print();

  // 默认析构 _ZN5EmptyD2Ev

  // B b1(1);
  // B b2(2);
  // b2 = b1;
}