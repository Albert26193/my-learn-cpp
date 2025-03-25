#include <iostream>
#include <string>

// 第一种实现：使用成员函数的乘法运算符（存在限制）
class RationalMember {
  public:
  // 构造函数：允许隐式转换
  RationalMember(int numerator = 0, int denominator = 1)
    : num_(numerator)
    , den_(denominator) {
    if (denominator == 0)
      throw std::invalid_argument("Denominator cannot be zero");
  }

  // Getter 函数
  [[nodiscard]] auto numerator() const -> int { return num_; }
  [[nodiscard]] auto denominator() const -> int { return den_; }

  // 成员函数版本的乘法运算符
  auto operator*(const RationalMember& rhs) const -> const RationalMember {
    return RationalMember(num_ * rhs.num_, den_ * rhs.den_);
  }

  // 用于打印的辅助函数
  void print() const { std::cout << num_ << "/" << den_; }

  private:
  int num_;   // 分子
  int den_;   // 分母
};

// 第二种实现：使用非成员函数的乘法运算符（更灵活）
class RationalNonMember {
  public:
  // 构造函数：允许隐式转换
  RationalNonMember(int numerator = 0, int denominator = 1)
    : num_(numerator)
    , den_(denominator) {
    if (denominator == 0)
      throw std::invalid_argument("Denominator cannot be zero");
  }

  // Getter 函数 - 注意这里使用公有接口而不是友元函数
  [[nodiscard]] auto numerator() const -> int { return num_; }
  [[nodiscard]] auto denominator() const -> int { return den_; }

  // 用于打印的辅助函数
  void print() const { std::cout << num_ << "/" << den_; }

  private:
  int num_;   // 分子
  int den_;   // 分母
};

// 非成员函数版本的乘法运算符
// 注意：不需要声明为友元，因为只使用公有接口
auto operator*(const RationalNonMember& lhs, const RationalNonMember& rhs)
  -> const RationalNonMember {
  return RationalNonMember(
    lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}

// 测试函数：展示成员函数版本的限制
void testMemberOperator() {
  std::cout << "\n=== 测试成员函数版本 ===\n";

  RationalMember half(1, 2);
  std::cout << "half = ";
  half.print();
  std::cout << "\n";

  // 正面例子：左操作数是 Rational 对象
  std::cout << "half * 2 = ";
  auto result1 = half * 2;   // 可以工作：half.operator*(2)
  result1.print();
  std::cout << "\n";

  // 反面例子：左操作数是整数
  std::cout << "2 * half = ";
  // auto result2 = 2 * half;  // 编译错误：2.operator*(half) 不存在
  std::cout << "编译错误！\n";
}

// 测试函数：展示非成员函数版本的灵活性
void testNonMemberOperator() {
  std::cout << "\n=== 测试非成员函数版本 ===\n";

  RationalNonMember half(1, 2);
  std::cout << "half = ";
  half.print();
  std::cout << "\n";

  // 例子1：左操作数是 Rational 对象
  std::cout << "half * 2 = ";
  auto result1 = half * 2;   // 可以工作
  result1.print();
  std::cout << "\n";

  // 例子2：左操作数是整数
  std::cout << "2 * half = ";
  auto result2 = 2 * half;   // 现在也可以工作了！
  result2.print();
  std::cout << "\n";

  // 例子3：两个有理数相乘
  RationalNonMember third(1, 3);
  std::cout << "half * third = ";
  auto result3 = half * third;
  result3.print();
  std::cout << "\n";
}

auto main() -> int {
  try {
    // 测试两种实现方式
    testMemberOperator();
    testNonMemberOperator();

    // 总结：
    std::cout << "\n=== 结论 ===\n";
    std::cout << "1. 成员函数版本：只有左操作数可以进行隐式转换\n";
    std::cout << "2. 非成员函数版本：两个操作数都可以进行隐式转换\n";
    std::cout << "3. 非成员函数提供了更好的灵活性和对称性\n";

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}