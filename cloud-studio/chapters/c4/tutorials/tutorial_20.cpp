#include <iostream>
using namespace std;

// 内置类型示例 - 使用 pass by value
void processInt(int x) {
  cout << "Processing int by value: " << x << endl;
}

// STL 迭代器示例 - 使用 pass by value
template<typename Container> void processIterator(typename Container::iterator it) {
  cout << "Processing iterator by value" << endl;
}

// 自定义类型示例 - 使用 pass-by-reference-to-const
class Widget {
  private:
  int id;

  public:
  Widget(int i)
    : id(i) {
    cout << "Widget " << id << " constructed" << endl;
  }
  Widget(const Widget& w)
    : id(w.id) {
    cout << "Widget " << id << " copied" << endl;
  }
  ~Widget() { cout << "Widget " << id << " destroyed" << endl; }

  [[nodiscard]] auto getId() const -> int { return id; }
};

void processWidget(const Widget& w) {
  cout << "Processing widget by reference-to-const: " << w.getId() << endl;
}

void processWidgetNoConst(Widget& w) {
  cout << "Processing widget by reference: " << w.getId() << endl;
}

auto main() -> int {
  // 内置类型 - 使用 pass by value
  int num = 42;
  processInt(num);

  cout << endl;
  // 自定义类型 - 使用 pass-by-reference-to-const
  Widget w(100);
  processWidget(w);   // 不会触发复制构造函数

  cout << endl;
  Widget w2(200);
  // 重新构造了一次
  processWidgetNoConst(w2);
  return 0;
}
