#include <iostream>
using namespace std;

class Number {
  public:
  int value_;
  Number(int value)
    : value_(value) {
    cout << "alloc value: " << value << endl;
  }

  auto operator*(const Number& other) -> Number {
    cout << "operation without const" << endl;
    return {value_ * other.value_};
  }

  // 第1个
  // const（参数中的）：保证函数不会修改传入的参数
  // 函数声明末尾的 const（第2个 const）
  // 修饰整个成员函数，表示这个函数是一个常量成员函数，不会修改调用对象（即 this
  // 指向的对象）的任何成员变量。
  // 第3个
  // const（函数声明末尾的）：保证函数不会修改调用该函数的对象，并允许常量对象调用该函数
  auto operator*(const Number& other) const -> const Number {
    cout << "operation with const" << endl;
    return Number(value_ * other.value_);
  }

  // 非const成员函数 - 可以修改成员变量
  void increment() {
    value_++;   // 合法：可以修改value_
  }

  // const成员函数 - 不能修改成员变量
  void tryToIncrement() const {
    // value_++;  // 编译错误！不能在const成员函数中修改成员变量
    // 如果取消注释上面这行，编译器会报错
  }

  // const成员函数 - 只能读取成员变量
  [[nodiscard]] auto getValue() const -> int {
    return value_;   // 合法：可以读取value_
  }
};

auto main() -> int {
  Number num1(3);
  Number num2(4);
  Number num3(1);

  const Number result = num1 * num2;   // without const
  (num1 * num2)       = num3;          // without const

  const Number num4(4);
  const Number num5(1);
  num4*        num5;   // with const

  return 0;
}