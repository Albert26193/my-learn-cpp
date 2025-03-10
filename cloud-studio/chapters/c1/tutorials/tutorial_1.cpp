#include <iostream>
using namespace std;

class UserDefinedType {
public:
  int value;
  UserDefinedType() {
    value = 0;
    cout << "UserDefinedType default construct." << endl;
  }
  UserDefinedType(const UserDefinedType &obj) {
    value = obj.value;
    cout << "[notice:] UserDefinedType copy construct." << endl;
  }
  ~UserDefinedType() { cout << "[notice:] UserDefinedType dealloc." << endl; }
};

// Good
void set_pass_by_reference_const(const UserDefinedType &obj) {
  cout << "set_pass_by_reference_const, obj.value: " << obj.value << endl;
}

// Bad，会调用拷贝构造函数
void set_pass_by_value(UserDefinedType obj) {
  cout << "set_pass_by_value, obj.value: " << obj.value << endl;
}

// Bad
void set_pass_by_reference(UserDefinedType &obj) {
  cout << "set_pass_by_reference, obj.value: " << obj.value << endl;
}

int main() {
  UserDefinedType obj1;
  obj1.value = 1;
  set_pass_by_reference_const(obj1);
  set_pass_by_value(obj1);
  set_pass_by_reference(obj1);
  return 0;
}