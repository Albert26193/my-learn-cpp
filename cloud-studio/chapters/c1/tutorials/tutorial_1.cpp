#include <iostream>
using namespace std;

class UserDefinedType {
public:
    int value{0};
    // 默认构造函数
    UserDefinedType() { cout << "UserDefinedType default construct." << endl; }

    // 拷贝构造函数
    // 初始化列表
    UserDefinedType(const UserDefinedType& obj)
      : value(obj.value) {
      cout << "[notice:] UserDefinedType copy construct." << endl;
    }

    // 析构函数
    ~UserDefinedType() { cout << "[notice:] UserDefinedType dealloc." << endl; }
};

// Good
void set_pass_by_reference_const(const UserDefinedType &obj) {
  cout << "set_pass_by_reference_const, obj.value: " << obj.value << endl;
}

// Bad，会调用拷贝构造函数
// Q：为什么不好？
// A：会调用拷贝构造函数，多一次拷贝, 对于自定义类型，鼓励
// pass-by-reference-const
// 这里是值传递， 还会调用析构函数
void set_pass_by_value(UserDefinedType obj) {
  cout << "set_pass_by_value, obj.value: " << obj.value << endl;
}

// Bad
// Q: 为什么不好？
// A：对于能用 const 的地方，尽量使用 const
void set_pass_by_reference(UserDefinedType &obj) {
  cout << "set_pass_by_reference, obj.value: " << obj.value << endl;
}

// 尾置返回类型
// trailing return type
// 好处有3点：
// 1. 便于模板编程
// 2. 可读性一定程度获得提高
// 3. lambda 表达式的一致性，比如
//    auto lambda = [](int x) -> int { return x * 2; };
auto main() -> int {
  UserDefinedType obj1;
  obj1.value = 1;
  set_pass_by_reference_const(obj1);
  cout << endl;
  set_pass_by_value(obj1);
  cout << endl;
  set_pass_by_reference(obj1);
  cout << endl;
  // 这里隐式调用析构函数
  return 0;
}