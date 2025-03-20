#include <iostream>
#include <vector>
using namespace std;


class B;

class A {
  friend B;

  public:
  A(int v)
    : a(v) {}
  ~A() = default;


  private:
  auto operator=(const A& rhs) -> A& {
    this->a = rhs.a;
    return *this;
  }
  int a;
};

class B {
  public:
  B(int a, int b)
    : a(a)
    , b(b) {}
  ~B() = default;
  void assign() {
    a = b;   // 合法操作， B 是A的友元
  }

  private:
  A a;
  A b;
};



class Uncopyable {
  public:
  Uncopyable()  = default;
  ~Uncopyable() = default;

  /**
    通过 = delete 显式告诉编译器这个函数是被禁用的
    如果代码中尝试拷贝构造这个类的对象，编译器会报错
   */
  Uncopyable(const Uncopyable&)                    = delete;
  auto operator=(const Uncopyable&) -> Uncopyable& = delete;
  // private:
  //     Uncopyable(const Uncopyable&);
  //     Uncopyable& operator=(const Uncopyable&);
};


class BankCard : public Uncopyable {
  // class BankCard{
  public:
  BankCard() = default;

  private:
  // BankCard(const BankCard& rhs);
  // BankCard& operator=(const BankCard& rhs);
  uint64_t balance;
};

auto main() -> int {
  BankCard card1{};
  BankCard card2{};
  // BankCard Card3(card1);   // 企图copy card1 ——应该禁止
  // card1 = card2;           // 企图copy card2 ——应该禁止

  auto b = B(1, 2);
  b.assign();

  return 0;
}
