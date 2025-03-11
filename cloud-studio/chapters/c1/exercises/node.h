#ifndef __NODE__H
#define __NODE__H

#include <bits/stdint-uintn.h>
#include <iostream>
#include <vector>

class BaseNode {
public:
    BaseNode(uint64_t id): id(id) {
        // std::cout << "创建 node" <<  id << "调用BaseNode构造函数" << std::endl;
    }
    virtual ~BaseNode() {
      std::cout << "删除 node " << id << "调用BaseNode析构函数" << std::endl;
    }
    uint64_t get_id() const {
        return id;
    }

    static void inc_count() {
        count++;
    }

    static void dec_count() {
        count--;
    }

    static uint32_t get_count() {
        return count;
    }



private:
    uint64_t id;
    static uint32_t count;
};



class PageNode: public BaseNode {
public:
    PageNode(uint64_t id): BaseNode(id) {
        inc_count();
        // std::cout << "创建 node" <<  id << "调用PageNode构造函数" << std::endl;
    }
    ~PageNode() {
        dec_count();
        std::cout <<"删除 node " <<  get_id() << "调用PageNode析构函数" << std::endl;
    }

};

class RectangleNode: public BaseNode {
public:
    RectangleNode(uint64_t id): BaseNode(id) {
        inc_count();
        width = new uint32_t(5);
        height = new uint32_t(10);
        // std::cout << "创建 node" <<  id << "调用RectangleNode构造函数" << std::endl;
    }
    ~RectangleNode() {
        dec_count();
        delete width;
        delete height;
        width = height = nullptr;
        std::cout <<"删除 node " <<  get_id() << "调用RectangleNode析构函数" << std::endl;
    }

    uint32_t get_area() const{
        return *width * *height;
    }


private:
    uint32_t* width;
    uint32_t* height;

};

enum NodeType {PageNodeType = 1, RectangleNodeType};

BaseNode* create_node(NodeType type);

#endif