# 07 为多态基类声明虚析构函数

## 问题引入：不当的析构函数设计

如果基类的析构函数不是虚函数（virtual），当通过基类指针删除派生类对象时会发生什么？
- 结果是未定义的（undefined behavior）
- 通常会导致派生类部分的资源没有被正确释放
- 可能引发内存泄漏或其他更严重的问题

## 示例代码

```cpp
class BaseNode {
public:
    BaseNode(uint64_t id): id(id) {}
    virtual ~BaseNode() {  // 声明为虚析构函数
        std::cout << "删除 node " << id << " 调用 BaseNode 析构函数" << std::endl;
    }   
private:
    uint64_t id;
};

class PageNode: public BaseNode { /* ... */ };
class RectangleNode: public BaseNode { /* ... */ };

int main() {
    auto node1 = create_node(PageNodeType);
    auto node2 = create_node(RectangleNodeType);
    
    // 正确：通过基类指针删除派生类对象
    auto* node3 = new RectangleNode(101);
    BaseNode* node3_cast = static_cast<BaseNode*>(node3);

    std::cout << "长方形面积 before delete: " << node3->get_area() << std::endl;
    
    delete node1;
    delete node2;
    delete node3_cast;  // 正确行为：会调用 RectangleNode 和 BaseNode 的析构函数
    
    // 不再是未定义行为
    // std::cout << "长方形面积 after delete: " << node3->get_area() << std::endl;
    
    std::cout << "BaseNode 大小: " << sizeof(BaseNode) << std::endl;
    return 0;
}
```

## 虚析构函数的使用原则

### 何时使用虚析构函数？

1. **需要虚析构函数的场景**
   - 类设计用作基类（base class）
   - 类中至少有一个虚函数
   - 类需要支持多态行为

2. **不需要虚析构函数的场景**
   - 类不打算作为基类
   - 类不需要支持多态
   - 类是工具类或值类型（如 `std::string`）

### 使用虚析构函数的代价

1. **内存开销**
   - 每个包含虚函数的类都会有一个虚函数表（virtual table）
   - 每个对象都会包含一个指向虚函数表的指针（vptr）

2. **性能影响**
   - 虚函数调用比普通函数调用稍慢
   - 对象大小增加可能影响缓存效率
   - 在 64 位系统上，可能无法将对象存入单个寄存器

## 最佳实践

1. 基类的析构函数应该是虚函数或受保护的非虚函数[^1]
2. 只在必要时使用虚析构函数（类中已有其他虚函数时）
3. 不要继承不带虚函数的类，除非是为了复用实现（如 `Uncopyable`）
4. **如果基类的析构函数是虚函数，派生类的析构函数自动成为虚函数**

[^1]: "受保护的非虚函数"是一种特殊的析构函数设计模式，用于防止通过基类指针删除派生类对象。具体来说：

```cpp
class Base {
protected:
    ~Base() { }  // 受保护的非虚析构函数
public:
    // ... 其他公共接口
};

class Derived : public Base {
public:
    ~Derived() { }  // 公有析构函数
};

int main() {
    Base* b = new Derived();  // 编译错误！
    delete b;                 // 无法通过基类指针删除

    Derived* d = new Derived();  // 正确
    delete d;                    // 正确
}
```

这种设计的优点：
1. 防止通过基类指针错误删除对象（避免未定义行为）
2. 不需要虚函数表，节省内存
3. 明确表明类不支持多态删除

使用场景：
- 当类不需要多态删除时
- 想要优化内存使用时
- 作为接口类但不需要虚析构函数时

## 编译和运行

```bash
clang++ -std=c++11 -Wall -Wextra tutorial_07.cpp -o app && ./app