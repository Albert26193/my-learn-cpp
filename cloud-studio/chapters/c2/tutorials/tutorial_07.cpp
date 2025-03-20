#include <bits/stdint-uintn.h>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;


class BaseNode {
  public:
  BaseNode(uint64_t id)
    : id(id) {
    // std::cout << "创建 node" <<  id << "调用BaseNode构造函数" << std::endl;
  }
  // virtual ~BaseNode() { std::cout << "删除 node " << id << "调用BaseNode析构函数" << std::endl; }

  // 问题：非虚析构函数会导致通过基类指针删除时的未定义行为
  ~BaseNode() { std::cout << "删除 node " << id << "调用BaseNode析构函数" << std::endl; }

  auto get_id() -> uint64_t const { return id; }

  private:
  uint64_t id;
};

class PageNode : public BaseNode {
  public:
  PageNode(uint64_t id)
    : BaseNode(id) {
    // std::cout << "创建 node" <<  id << "调用PageNode构造函数" << std::endl;
  }
  ~PageNode() { std::cout << "删除 node " << get_id() << "调用PageNode析构函数" << std::endl; }
};

class RectangleNode : public BaseNode {
  public:
  RectangleNode(uint64_t id)
    : BaseNode(id)
    , width(new uint32_t(5))
    , height(new uint32_t(10)) {
    // std::cout << "创建 node" <<  id << "调用RectangleNode构造函数" << std::endl;
  }
  ~RectangleNode() {
    delete width;
    delete height;
    width = height = nullptr;
    std::cout << "删除 node " << get_id() << "调用RectangleNode析构函数" << std::endl;
  }

  auto get_area() -> unsigned int const { return *width * *height; }

  private:
  uint32_t* width;
  uint32_t* height;
};

enum NodeType { PageNodeType = 1, RectangleNodeType };

auto create_node(NodeType type) -> BaseNode* {
  static uint64_t id = 0;

  switch (type) {
  case PageNodeType:
    return new PageNode(++id);
    break;
  case RectangleNodeType:
    return new RectangleNode(++id);
    break;
  default:
    std::cout << "无法创建" << type << "类型的节点，将返回空指针" << std::endl;
  }

  return nullptr;
}


// auto main() -> int {
//   auto  node1 = create_node(PageNodeType);
//   auto  node2 = create_node(RectangleNodeType);
//   auto* node3 = new RectangleNode(101);
//   // BaseNode* node3_cast = static_cast<BaseNode*>(node3);
//   // std::cout << "长方形面积before delete: " << node3->get_area() << std::endl;
//   delete node1;
//   delete node2;
//   delete node3;
//   // delete node3_cast;
//   // std::cout << "长方形面积after delete: " << node3->get_area() << std::endl;

//   std::cout << "BaseNode: " << sizeof(BaseNode) << std::endl;

//   return 0;
// }

auto main() -> int {
  std::cout << "=== 通过派生类指针删除（正确的行为）===" << std::endl;
  RectangleNode* derived = new RectangleNode(1);
  delete derived;   // 正确：会调用 RectangleNode 和 BaseNode 的析构函数

  std::cout << "\n=== 通过基类指针删除（错误的行为）===" << std::endl;
  BaseNode* base = new RectangleNode(2);
  delete base;   // 问题：只会调用 BaseNode 的析构函数，导致内存泄漏

  std::cout << "\nBaseNode大小（无虚函数表）: " << sizeof(BaseNode) << " 字节" << std::endl;
  return 0;
}