#include <iostream>
#include <memory>

// 错误示例：无法访问原始资源
class BadCase {
  public:
  BadCase()
    : ptr_(new int(0)) {}
  ~BadCase() { delete ptr_; }

  // 没有提供对原始资源的访问方法

  private:
  int* ptr_;
};

// 正确示例：提供对原始资源的访问
class GoodCase {
  public:
  GoodCase()
    : ptr_(new int(0)) {}
  ~GoodCase() { delete ptr_; }

  // 提供对原始资源的访问
  int* get() const { return ptr_; }

  private:
  int* ptr_;
};

// 额外示例：使用重载操作符访问原始资源
class BetterCase {
  public:
  BetterCase()
    : ptr_(new int(0)) {}
  ~BetterCase() { delete ptr_; }

  // 方法1：get() 函数
  int* get() const { return ptr_; }

  // 方法2：重载解引用操作符
  int& operator*() const { return *ptr_; }

  // 方法3：重载箭头操作符（对于指向对象的指针更有用）
  int* operator->() const { return ptr_; }

  private:
  int* ptr_;
};

// 额外示例：使用转换操作符
class ExplicitCase {
  public:
  ExplicitCase()
    : ptr_(new int(0)) {}
  ~ExplicitCase() { delete ptr_; }

  // 显式转换操作符（更安全）
  explicit operator int*() const { return ptr_; }

  private:
  int* ptr_;
};

class ImplicitCase {
  public:
  ImplicitCase()
    : ptr_(new int(0)) {}
  ~ImplicitCase() { delete ptr_; }

  // 隐式转换操作符（方便但可能导致意外转换）
  operator int*() const { return ptr_; }

  private:
  int* ptr_;
};

// 使用智能指针的现代实现
class ModernCase {
  public:
  ModernCase()
    : ptr_(std::make_unique<int>(0)) {}
  // 不需要显式析构函数，智能指针会自动管理资源

  // 提供对原始资源的访问
  int* get() const { return ptr_.get(); }

  // 获取智能指针本身（可能需要共享所有权时）
  const std::unique_ptr<int>& getPtr() const { return ptr_; }

  private:
  std::unique_ptr<int> ptr_;
};

int main() {
  // 无法访问原始资源的例子
  BadCase bad;
  // 下面的代码无法编译，因为无法访问 ptr_
  // *bad.ptr_ = 42;

  // 使用 get() 方法访问原始资源
  GoodCase good;
  *good.get() = 42;
  std::cout << "GoodCase value: " << *good.get() << std::endl;

  // 使用重载操作符访问原始资源
  BetterCase better;
  *better = 100;   // 使用重载的 * 操作符
  std::cout << "BetterCase value: " << *better.get() << std::endl;

  // 使用显式转换
  ExplicitCase exp;
  *static_cast<int*>(exp) = 200;
  std::cout << "ExplicitCase value: " << *static_cast<int*>(exp) << std::endl;

  // 使用隐式转换
  ImplicitCase imp;
  int*         raw_ptr = imp;   // 隐式转换
  *raw_ptr             = 300;
  std::cout << "ImplicitCase value: " << *raw_ptr << std::endl;

  // 使用现代 C++ 智能指针
  ModernCase modern;
  *modern.get() = 400;
  std::cout << "ModernCase value: " << *modern.get() << std::endl;

  return 0;
}