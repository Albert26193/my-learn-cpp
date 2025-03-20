# C++ 核心概念总结

## 目录
1. [类的内存模型](./cpp_class_memory_model.md)
2. [内存与继承](./cpp_memory_and_inheritance.md)
3. [资源所有权管理](./resource_ownership.md)
4. [头文件保护](./header_guards.md)
5. [引用与指针](./references_vs_pointers.md)

## 快速导航

### 1. 内存布局
- [程序的内存分区](./cpp_memory_and_inheritance.md#1-c-程序的内存布局)
- [类的内存模型](./cpp_class_memory_model.md#2-c-class-内存模型)
- [对象的内存布局](./cpp_class_memory_model.md#23-内存布局详解)

### 2. 继承与多态
- [虚析构函数](./cpp_memory_and_inheritance.md#3-虚析构函数问题)
- [非虚析构的问题](./cpp_memory_and_inheritance.md#31-非虚析构函数的问题)
- [解决方案](./cpp_memory_and_inheritance.md#32-解决方案)

### 3. 资源管理
- [智能指针使用](./resource_ownership.md)
- [RAII原则](./resource_ownership.md)
- [内存泄漏预防](./resource_ownership.md)

### 4. 最佳实践
- [类设计优化](./cpp_class_memory_model.md#4-内存优化建议)
- [内存管理](./cpp_memory_and_inheritance.md#4-编程最佳实践)
- [代码组织](./header_guards.md)

## 核心要点总结

1. **内存分布**：
   ```mermaid
   graph TB
       A[程序内存布局] --> B[代码段]
       A --> C[数据段]
       A --> D[堆区]
       A --> E[栈区]
   ```

2. **类的内存模型**：
   ```mermaid
   graph TD
       A[类的内存组成] --> B[代码部分<br>不占对象内存]
       A --> C[数据部分<br>占用对象内存]
       
       B --> D[成员函数<br>存储在代码段]
       B --> E[虚函数实现<br>存储在代码段]
       
       C --> F[数据成员<br>存储在对象中]
       C --> G[vptr<br>如果有虚函数]
   ```

3. **关键概念**：
   - 成员函数不占用对象内存
   - 虚函数表指针（vptr）占用对象内存
   - 对象大小受内存对齐影响
   - 静态成员不占用对象内存

4. **最佳实践**：
   - 合理使用虚函数
   - 注意内存对齐
   - 优化类的内存布局
   - 使用智能指针管理资源

## 相关代码示例

```cpp
// 类的内存布局示例
class Example {
public:
    void method() { }         // 在代码段，不占内存
    virtual void vf() { }     // 在代码段，但添加vptr
    static void sf() { }      // 在代码段，不占内存
    
private:
    int x;                    // 4字节
    static int count;         // 在数据段，不占内存
    double value;             // 8字节
    char flag;                // 1字节+填充
};
```

## 进一步学习

1. **深入理解**：
   - 虚函数表的实现
   - 内存对齐的细节
   - 编译器优化技术

2. **实践建议**：
   - 使用工具查看对象布局
   - 分析内存使用模式
   - 注意性能关键点

3. **常见陷阱**：
   - 虚析构函数的重要性
   - 内存泄漏的预防
   - 对象切片问题
