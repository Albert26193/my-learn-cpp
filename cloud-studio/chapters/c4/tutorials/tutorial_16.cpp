#include <iostream>


int main() {
  // Good Case
  std::string* strArr1 = new std::string;
  std::string* strArr2 = new std::string[100];

  delete strArr1;
  delete[] strArr2;

  // Bad Case
  // std::string* strArr = new std::string[100];
  // delete strArr;

  // delete strArr 发生了什么
  // 1. 只会调用第一个元素的析构函数，其余99个元素不会被析构
  // 2. 可能导致内存泄漏，因为其余元素占用的内存没有被正确释放
  // 3. 对于非POD类型如std::string，这会导致资源泄漏（如字符串缓冲区）
  // 4. 在某些情况下可能导致未定义行为
}