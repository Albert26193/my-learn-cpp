#include <cstdio>
#include <iostream>

class Base {
  public:
  Base() {
    printf("base constructor\n");
    foo();
  }
  virtual ~Base() {
    printf("base destructor \n");
    foo();
  }
  virtual void foo() const { printf("base foo \n"); }
};

class Derived : public Base {
  public:
  Derived() {
    printf("derived constructor \n");
    foo();
  }
  ~Derived() {
    printf("derived destructor \n");
    // foo();
    Base::foo();
  }
  void foo() const override { printf("derived foo \n"); }
};

int main() {
  Derived* ptr = new Derived;
  std::cout << std::endl;
  delete ptr;
  return 0;
}