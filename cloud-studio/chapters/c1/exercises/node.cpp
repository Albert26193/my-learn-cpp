#include "node.h"

uint32_t BaseNode::count = 0;

BaseNode* create_node(NodeType type){
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