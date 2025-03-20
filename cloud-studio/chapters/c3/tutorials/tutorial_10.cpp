#include <iostream>

template<typename T> class Vector3 {
  public:
  T x, y, z;

  Vector3(T x = 0, T y = 0, T z = 0)
    : x(x)
    , y(y)
    , z(z) {}

  auto operator=(const Vector3& other) -> Vector3& {
    x = other.x;
    y = other.y;
    z = other.z;
    // 这里为什么需要写 return *this;
    // 因为需要修改对象的状态
    // 且支持链式调用，比如 a = b = c
    return *this;
  }

  auto operator+=(const Vector3& other) -> Vector3& {
    x += other.x;
    y += other.y;
    z += other.z;
    // 将来需要 v1 = (v2 += v3);
    return *this;
  }

  auto operator+(const Vector3& other) const -> Vector3 {
    return Vector3<T>(x + other.x, y + other.y, z + other.z);
  }

  void print() const { std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl; }
};

typedef Vector3<int> Vector3i;

auto main() -> int {
  Vector3i v1(1, 2, 3);
  Vector3i v2(4, 5, 6);
  Vector3i v3(7, 8, 9);

  v1 = (v2 += v3);
  v1.print();

  v3 = v1 + v2 + v3;
  v3.print();

  return 0;
}