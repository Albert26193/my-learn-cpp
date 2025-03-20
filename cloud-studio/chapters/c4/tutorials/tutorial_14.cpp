#include <algorithm>
#include <iostream>
#include <memory>


class BadCase {
  public:
  BadCase(size_t size)
    : data_(new int[size]) {}

  // 拷贝构造函数
  // 这里使用浅拷贝，本质上是错误的
  BadCase(const BadCase& other)
    : data_(other.data_) {
    // 错误：直接拷贝原始指针，导致两个对象指向同一块内存
  }

  // 拷贝赋值操作符
  auto operator=(const BadCase& other) -> BadCase& {
    data_ = other.data_;   // 错误：直接拷贝原始指针，导致两个对象指向同一块内存
    return *this;
  }

  auto sharesDataWith(const BadCase& other) -> bool const { return data_ == other.data_; }
  ~BadCase() { delete[] data_; }

  private:
  int* data_;
};

class GoodCase {
  public:
  GoodCase(size_t size)
    : size_(size) {
    data_ = new int[size_];
  }

  // 拷贝构造函数
  GoodCase(const GoodCase& other)
    : size_(other.size_) {
    data_ = new int[size_];
    // 深拷贝：分配新的内存并复制数据
    std::copy(other.data_, other.data_ + size_, data_);
  }

  // 拷贝赋值操作符
  auto operator=(const GoodCase& other) -> GoodCase& {
    if (this == &other) {
      return *this;   // 自赋值检查
    }

    delete[] data_;   // 释放原有内存

    size_ = other.size_;
    data_ = new int[size_];
    // 深拷贝：分配新的内存并复制数据
    std::copy(other.data_, other.data_ + size_, data_);

    return *this;
  }

  // 在 GoodCase 类中添加
  [[nodiscard]] auto sharesDataWith(const GoodCase& other) const -> bool {
    return data_ == other.data_;
  }

  ~GoodCase() { delete[] data_; }

  private:
  int*   data_;
  size_t size_;
};

auto main() -> int {
  GoodCase m1(10);
  // 调用拷贝构造函数，为 m2 分配新的内存并复制 m1 的数据
  GoodCase m2(m1);
  if (m1.sharesDataWith(m2)) {
    std::cout << "m1 has the same memory address with m2" << std::endl;
  }
  // 当 m1 和 m2 被销毁时，各自释放各自的内存，没有悬挂指针和重复释放内存的问题

  // 调用拷贝构造函数，m11 和 m22 指向同一块内存
  // 当 m22 被销毁时，会释放内存，但 m11 仍然持有已经被释放的内存的指针
  // 当 m11 被销毁时，会再次尝试释放同一块内存，导致未定义行为
  BadCase m11(10);
  BadCase m22(m11);
  if (m11.sharesDataWith(m22)) {
    std::cout << "m11 has the same memory address with m22" << std::endl;
  }
  return 0;
}