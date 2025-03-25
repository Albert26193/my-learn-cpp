#include <iostream>
#include <string>
#include <utility>
#include <vector>

// 命名空间用于组织相关代码
namespace WidgetStuff {

// WidgetImpl 类的实现
class WidgetImpl {
  public:
  WidgetImpl(int val = 0)
    : a(val)
    , b(val)
    , c(val) {
    // 模拟一个大型 vector
    v.resize(1000, val);
  }

  // 用于演示的打印函数
  void print() const {
    std::cout << "WidgetImpl: a=" << a << ", b=" << b << ", c=" << c << ", vector size=" << v.size()
              << std::endl;
  }

  private:
  int                 a, b, c;
  std::vector<double> v;   // 假设这是一个很大的 vector
};

// Widget 类 - 使用 pimpl 模式
class Widget {
  public:
  // 构造函数
  Widget(int val = 0)
    : pImpl(new WidgetImpl(val)) {}

  // 析构函数
  ~Widget() { delete pImpl; }

  // 拷贝构造函数
  Widget(const Widget& rhs)
    : pImpl(new WidgetImpl(*rhs.pImpl)) {
    std::cout << "Copy constructor called (expensive!)" << std::endl;
  }

  // 拷贝赋值运算符
  auto operator=(const Widget& rhs) -> Widget& {
    std::cout << "Copy assignment called (expensive!)" << std::endl;
    if (this != &rhs) {
      *pImpl = *rhs.pImpl;   // 深拷贝，开销大
    }
    return *this;
  }

  // 成员版本的 swap 函数 - 高效且不抛出异常
  void swap(Widget& other) noexcept {
    std::cout << "Efficient member swap called" << std::endl;
    std::swap(pImpl, other.pImpl);   // 只交换指针
  }

  // 用于演示的打印函数
  void print() const { pImpl->print(); }

  private:
  WidgetImpl* pImpl;   // 指向实现的指针
};

// non-member swap 函数（在同一命名空间内）
void swap(Widget& a, Widget& b) noexcept {
  std::cout << "Non-member swap in WidgetStuff namespace called" << std::endl;
  a.swap(b);
}

}   // namespace WidgetStuff

// 特化 std::swap - 修复异常规范
namespace std {
template<> void swap(WidgetStuff::Widget& a, WidgetStuff::Widget& b) {
  std::cout << "Specialized std::swap called" << std::endl;
  a.swap(b);
}
}   // namespace std

// 演示不同的 swap 调用方式
template<typename T> void demonstrateSwap(T& obj1, T& obj2) {
  std::cout << "\n=== Before swap ===" << std::endl;
  std::cout << "obj1: ";
  obj1.print();
  std::cout << "obj2: ";
  obj2.print();

  // 推荐的调用方式
  using std::swap;    // 使 std::swap 可见
  swap(obj1, obj2);   // 编译器会选择最合适的版本

  std::cout << "\n=== After swap ===" << std::endl;
  std::cout << "obj1: ";
  obj1.print();
  std::cout << "obj2: ";
  obj2.print();
}

auto main() -> int {
  using namespace WidgetStuff;

  // 创建两个 Widget 对象
  Widget w1(1);
  Widget w2(2);

  std::cout << "=== Demonstrating recommended swap usage ===" << std::endl;
  demonstrateSwap(w1, w2);

  std::cout << "\n=== Demonstrating std::swap directly (not recommended) ===" << std::endl;
  std::swap(w1, w2);   // 仍然会调用特化版本

  return 0;
}
