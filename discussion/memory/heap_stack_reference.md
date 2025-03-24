# C++ 内存管理：堆、栈与引用

## 1. 栈（Stack）内存

### 1.1 特点
- 自动分配和释放
- LIFO（后进先出）结构
- 大小通常较小（典型值为1MB-8MB）
- 分配和释放速度快
- 生命周期跟随作用域

### 1.2 典型用例
```cpp
void stackExample() {
    int number = 42;           // 栈上的整数
    double values[100];        // 栈上的数组
    std::string text;          // string对象在栈上，但其内部缓冲区在堆上
    Widget w;                  // 栈上的自定义对象
}  // 所有对象自动销毁
```

## 2. 堆（Heap）内存

### 2.1 特点
- 手动分配和释放（或使用智能指针）
- 大小受限于系统可用内存
- 分配和释放较慢
- 生命周期由程序员控制
- 可能产生内存碎片

### 2.2 典型用例
```cpp
// 传统方式（不推荐）
Widget* w1 = new Widget();     // 手动分配
delete w1;                     // 手动释放

// 现代方式（推荐）
auto w2 = std::make_unique<Widget>();  // 智能指针管理
auto w3 = std::make_shared<Widget>();  // 共享所有权
```

## 3. 引用与指针

### 3.1 引用的特点
- 必须初始化
- 不能重新绑定
- 不能为空
- 本质是常量指针

### 3.2 安全使用引用
```cpp
// 1. 函数参数（安全）
void process(const Widget& w) {
    w.doSomething();
}

// 2. 类成员引用（需要小心）
class Manager {
    Widget& managed_;  // 危险：需要确保引用对象的生命周期
public:
    Manager(Widget& w) : managed_(w) {}
};

// 3. 返回引用（需要格外小心）
const Widget& getWidget() {
    static Widget w;  // 静态对象是安全的
    return w;
}
```

## 4. 常见陷阱和最佳实践

### 4.1 返回栈上对象的引用（危险！）
```cpp
// 错误示例
const Widget& createWidget() {
    Widget w;        // 栈上对象
    return w;        // 危险：返回已销毁对象的引用
}

// 正确做法
Widget createWidget() {
    return Widget(); // 返回值（编译器会优化）
}
```

### 4.2 返回堆上对象的引用（内存泄漏！）
```cpp
// 错误示例
const Widget& createHeapWidget() {
    auto* w = new Widget();  // 堆上对象
    return *w;              // 危险：谁来delete？
}

// 正确做法
std::unique_ptr<Widget> createHeapWidget() {
    return std::make_unique<Widget>();
}
```

## 5. 智能指针的使用

### 5.1 unique_ptr
```cpp
// 独占所有权
auto p1 = std::make_unique<Widget>();
// p1 是唯一的所有者
// 不能复制，但可以移动
auto p2 = std::move(p1);  // p1现在为nullptr
```

### 5.2 shared_ptr
```cpp
// 共享所有权
auto p1 = std::make_shared<Widget>();
auto p2 = p1;  // 两个指针共享所有权
// 当最后一个shared_ptr销毁时，对象被删除
```

## 6. 最佳实践总结

1. **优先使用栈**：
   - 自动内存管理
   - 更好的局部性
   - 更少的内存问题

2. **需要动态分配时**：
   - 使用智能指针而不是裸指针
   - 优先使用 `unique_ptr`
   - 只在需要共享所有权时使用 `shared_ptr`

3. **引用使用原则**：
   - 用于函数参数（避免拷贝）
   - 用于明确生命周期的对象
   - 不要返回局部对象的引用

4. **内存管理策略**：
   - 明确所有权
   - 使用RAII
   - 避免内存泄漏
   - 避免悬垂引用

## 7. 性能考虑

1. **栈分配**：
   - 速度快
   - 缓存友好
   - 没有内存碎片

2. **堆分配**：
   - 分配/释放开销大
   - 可能导致内存碎片
   - 但大小更灵活

3. **智能指针开销**：
   - 控制块开销（shared_ptr）
   - 引用计数操作
   - 析构函数调用链
