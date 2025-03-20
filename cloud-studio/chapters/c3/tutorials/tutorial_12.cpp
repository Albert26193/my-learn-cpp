
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

class ArrayExample {
  public:
  int*   data;
  size_t size;

  ArrayExample(size_t size)
    : data(new int[size])
    , size(size) {}

  ~ArrayExample() { delete[] data; }

  ArrayExample(const ArrayExample& other)
    : data(new int[other.size])
    , size(other.size) {
    init(other);
  }

  // 这里需要复制每一个成员变量
  auto operator=(const ArrayExample& other) -> ArrayExample& {
    if (this != &other) {
      delete[] data;
      data = new int[other.size];
      size = other.size;
      init(other);
    }
    return *this;
  }

  auto init(const ArrayExample& other) -> void {
    std::copy(other.data, other.data + other.size, data);
  }

  auto print() const -> void {
    for (size_t i = 0; i < size; ++i) {
      std::cout << data[i] << ' ';
    }
    std::cout << '\n';
  }
};

class DerivedArrayExample : public ArrayExample {
  public:
  int extra;

  DerivedArrayExample(size_t size, int extra)
    : ArrayExample(size)
    , extra(extra) {}

  DerivedArrayExample(const DerivedArrayExample& other)
    : ArrayExample(other.size)
    , extra(other.extra) {
    init(other);
  }

  // 调用基类的拷贝赋值操作
  auto operator=(const DerivedArrayExample& other) -> DerivedArrayExample& {
    if (this != &other) {
      ArrayExample::operator=(other);
      extra = other.extra;
      init(other);
    }
    return *this;
  }

  auto init(const DerivedArrayExample& other) -> void {
    std::copy(other.data, other.data + other.size, data);
    extra = other.extra;
  }
};


auto main() -> int {
  DerivedArrayExample c(10, 1);
  c.data[1] = 5;
  c.print();

  DerivedArrayExample d(10, 1);
  d = c;
  d.print();

  return 0;
}