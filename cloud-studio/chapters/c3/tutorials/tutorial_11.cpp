#include <iostream>
#include <vector>

template<typename T> class Vector3 {
  public:
  T x, y, z;

  Vector3(T x = 0, T y = 0, T z = 0)
    : x(x)
    , y(y)
    , z(z)
    , lists(new T[3]{x, y, z}) {}

  ~Vector3() { delete[] lists; }

  auto operator=(const Vector3& other) -> Vector3& {
    // 处理自我赋值
    // 什么时候需要检查 `this != &other`？
    // 当类中有指针成员，需要手动分配和释放内存时
    // 什么时候不需要检查 `this != &other`？
    // 当类中只有基本类型的成员变量的操作时
    if (this != &other) {
      delete[] lists;
      lists = new T[3];
      for (size_t i = 0; i < 3; ++i) {
        lists[i] = other.lists[i];
      }
      x = other.x;
      y = other.y;
      z = other.z;
    }
    return *this;
  }

  auto getLists() -> T* { return lists; }

  void print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    std::cout << "(" << lists[0] << ", " << lists[1] << ", " << lists[2] << ")" << std::endl;
  }

  private:
  T* lists;
};

using Vector3i = Vector3<int>;

int main() {
  Vector3i v1(1, 2, 3);
  v1 = v1;
  v1.print();

  return 0;
}