#include <iostream>
#include <memory>
#include <stdexcept>

class Widget {};


int priority() {
  return 0;
}


void processWidget(std::shared_ptr<Widget> pw, int priority) {}


int main() {
  // 无法通过编译
  // processWidget(new Widget, priority());


  // 虽然能通过编译，但有可能造成内存泄漏
  // processWidget(std::shared_ptr<Widget>(new Widget), priority());

  // 使用分离语句
  std::shared_ptr<Widget> pw(new Widget);
  processWidget(pw, priority());
}
