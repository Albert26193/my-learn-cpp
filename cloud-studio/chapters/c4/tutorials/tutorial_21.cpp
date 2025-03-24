#include <iostream>
#include <string>

class Rational {
  public:
  // 构造函数不声明为explicit，允许隐式转换
  Rational(int numerator = 0, int denominator = 1)
    : n(numerator)
    , d(denominator) {
    std::cout << "Rational(" << n << "/" << d << ") constructed\n";
  }

  // 拷贝构造函数
  Rational(const Rational& other)
    : n(other.n)
    , d(other.d) {
    std::cout << "Rational(" << n << "/" << d << ") copied\n";
  }

  // 析构函数
  ~Rational() { std::cout << "Rational(" << n << "/" << d << ") destroyed\n"; }

  // 打印函数
  void print() const { std::cout << n << "/" << d; }

  private:
  int n, d;   // 分子(numerator)和分母(denominator)

  // 声明为友元，允许访问私有成员
  friend auto operator*(const Rational& lhs, const Rational& rhs) -> const Rational;
  friend auto badMultiplyStack(const Rational& lhs, const Rational& rhs) -> const Rational&;
  friend auto badMultiplyHeap(const Rational& lhs, const Rational& rhs) -> const Rational&;
};

// 正确实现：返回一个新对象
inline auto operator*(const Rational& lhs, const Rational& rhs) -> const Rational {
  std::cout << "\nCalling operator*\n";
  return Rational(lhs.n * rhs.n, lhs.d * rhs.d);
}

// 错误实现1：返回栈上对象的引用
// 不应该返回引用
inline auto badMultiplyStack(const Rational& lhs, const Rational& rhs) -> const Rational& {
  std::cout << "\nCalling badMultiplyStack\n";
  Rational result(lhs.n * rhs.n, lhs.d * rhs.d);   // 局部对象
  return result;                                   // 危险：返回局部对象的引用！
}

// 错误实现2：返回堆上对象的引用
// 不应该返回引用
inline auto badMultiplyHeap(const Rational& lhs, const Rational& rhs) -> const Rational& {
  std::cout << "\nCalling badMultiplyHeap\n";
  auto* result = new Rational(lhs.n * rhs.n, lhs.d * rhs.d);   // 堆对象
  return *result;                                              // 危险：内存泄漏！
}

void demonstrateGoodMultiply() {
  std::cout << "\n=== Good Multiply Demo ===\n";
  Rational a(1, 2);   // 1/2
  Rational b(3, 4);   // 3/4

  std::cout << "\nMultiplying a * b:\n";
  Rational c = a * b;   // 调用operator*

  std::cout << "\nResult: ";
  c.print();
  std::cout << "\n";
}

void demonstrateBadStackMultiply() {
  std::cout << "\n=== Bad Stack Multiply Demo ===\n";
  Rational a(1, 2);   // 1/2
  Rational b(3, 4);   // 3/4

  std::cout << "\nMultiplying a * b using badMultiplyStack:\n";
  const Rational& c = badMultiplyStack(a, b);   // 危险！

  std::cout << "\nTrying to use the result: ";
  c.print();   // 未定义行为：访问已销毁对象
  std::cout << "\n";
}

void demonstrateBadHeapMultiply() {
  std::cout << "\n=== Bad Heap Multiply Demo ===\n";
  Rational a(1, 2);   // 1/2
  Rational b(3, 4);   // 3/4

  std::cout << "\nMultiplying a * b using badMultiplyHeap:\n";
  const Rational& c = badMultiplyHeap(a, b);   // 内存泄漏！

  std::cout << "\nResult exists but leaks memory: ";
  c.print();
  std::cout << "\n";
  // c指向的内存永远不会被释放
}

auto main() -> int {
  std::cout << "Demonstrating different ways to return objects:\n";

  // 1. 演示正确的实现
  demonstrateGoodMultiply();

  // 2. 演示错误的栈实现（可能导致崩溃）
  demonstrateBadStackMultiply();

  // 3. 演示错误的堆实现（导致内存泄漏）
  demonstrateBadHeapMultiply();

  return 0;
}
