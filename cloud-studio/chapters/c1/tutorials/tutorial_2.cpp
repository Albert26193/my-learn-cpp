#include <iostream>

#define CALL_WITH_MAX(a, b) a > b ? a : b

int main() {
  int num1 = 4;
  int num2 = 2;

  // Bad，num会自增两次
  int result = CALL_WITH_MAX(++num1, num2);

  std::cout << "result: " << result << std::endl;
  std::cout << "Expected num1: " << 5 << std::endl;
  std::cout << "Actual num1: " << num1 << std::endl;

  return 0;
}