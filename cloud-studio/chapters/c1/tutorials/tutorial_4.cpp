#include <iostream>
#include <utility>

class MyClass {
  public:
  const int value0 = 0;

  double value1;

  // 构造函数中初始化成员变量
  MyClass(int val)
    : value2(val)
    , value0(0) {
    // value0 = 9; //会报错
    std::cout << "在构造函数中初始化" << std::endl;
  }

  // 构造函数中赋值成员变量
  /**
    这里使用std::move(str_val)进行移动赋值有几个重要原因：
    - 性能优化：
      str_val是一个std::string参数，如果直接赋值会发生字符串的拷贝操作
      使用std::move将str_val转换为右值，这样可以触发移动语义
      移动操作只是转移字符串的所有权，避免了不必要的内存分配和数据拷贝
    - 资源管理：
      str_val是构造函数的局部参数，在构造函数结束后就会被销毁
      **既然参数马上就要被销毁了，那么直接"偷走"它的内容是最高效的做法**
      移动后的str_val会处于有效但未指定的状态，这完全没问题因为我们不会再使用它
    - 最佳实践：
      在C++11及以后的版本中，对于不再需要使用的string等资源型对象，推荐使用移动语义
      这是现代C++的一个重要特性，可以显著提升性能，特别是在处理大型字符串或容器时
      这种写法体现了现代C++的高效资源管理理念，避免了不必要的拷贝开销，是一种推荐的编程实践。
   */
  MyClass(std::string str_val, int int_val)
    : value1(0.0)
    , value2(int_val)
    , value3(std::move(str_val)) {
    std::cout << "在构造函数中赋值" << std::endl;
  }

  // 获取成员变量值的方法
  [[nodiscard]] auto GetValue1() -> double const { return value1; }

  [[nodiscard]] auto GetValue2() -> int const { return value2; }

  private:
  int         value2;
  std::string value3;
};

auto main() -> int {
  MyClass obj1(20);
  MyClass obj2("20", 20);
  std::cout << "读取未初始化 value1: " << obj1.GetValue1() << std::endl;
  return 0;
}