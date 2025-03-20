#include <bits/stdint-uintn.h>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>
using namespace std;

class A {
  public:
  static uint32_t count;
  A() { id = ++count; }

  void id_is_five() {
    std::cout << "A" << id << "判断id是否为5" << std::endl;
    if (id == 5) {
      throw std::runtime_error("test throw an exception in function");
    }
  }

  ~A() noexcept {
    std::cout << "析构 A" << id << std::endl;
    try {
      if (id == 5) {
        throw std::runtime_error("test throw an exception in deconstructor");
      }
    } catch (exception e) {
      std::cout << "析构时发生异常: " << e.what() << std::endl;
    }
  }

  private:
  int32_t id;
};

uint32_t A::count = 0;


void test() {
  auto vec = std::vector<A>(10);
}


auto main() -> int {
  test();

  return 0;

  //预期在函数结束时析构vec中的10个对象
}