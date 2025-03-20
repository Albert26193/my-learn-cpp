#include <iostream>
#include <memory>

using namespace std;

class Investment {
  public:
  void print() { cout << "Investment info" << endl; }
};

auto createInvestment() -> shared_ptr<Investment> {
  return make_shared<Investment>();
};

auto main() -> int {
  // pInv1 指向 Investment 对象
  std::shared_ptr<Investment> pInv1(createInvestment());

  // pInv2 指向 Investment 对象
  const std::shared_ptr<Investment>& pInv2(pInv1);

  pInv1->print();

  pInv2->print();

  // 此处返回是 true
  // 因为 pInv1 和 pInv2 指向同一个对象
  cout << (pInv1 == pInv2) << endl;

  return 0;
}