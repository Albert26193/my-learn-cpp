# C++ 栈上分配类的设计

## 基本原理

栈上分配类通过禁用动态内存分配相关的操作符来实现。主要包括：

1. **禁用 new/delete 操作符**：
```cpp
class StackOnly {
public:
    // 禁用动态内存分配
    static void* operator new(std::size_t) = delete;
    static void* operator new[](std::size_t) = delete;
    static void operator delete(void*) = delete;
    static void operator delete[](void*) = delete;
};
```

## 设计考虑

1. **构造函数**：
   - 保持公有，允许正常创建对象
   - 可以提供多种构造方式
   ```cpp
   StackOnly() = default;
   explicit StackOnly(int val) : value(val) {}
   ```

2. **拷贝和移动语义**：
   - 通常允许拷贝和移动
   - 使用 `= default` 使用编译器生成的版本
   ```cpp
   StackOnly(const StackOnly&) = default;
   StackOnly(StackOnly&&) = default;
   ```

3. **赋值操作**：
   - 同样允许拷贝和移动赋值
   ```cpp
   auto operator=(const StackOnly&) -> StackOnly& = default;
   auto operator=(StackOnly&&) -> StackOnly& = default;
   ```

## 使用限制

1. **不能使用 new 创建**：
```cpp
StackOnly* ptr = new StackOnly();  // 编译错误
```

2. **不能使用智能指针**：
```cpp
auto ptr = std::make_unique<StackOnly>();  // 编译错误
```

3. **允许的操作**：
```cpp
// 正确：栈上创建
StackOnly obj1;
StackOnly obj2(100);

// 正确：拷贝和赋值
StackOnly obj3 = obj2;
obj1 = obj2;

// 正确：函数返回值
auto createStackObject() -> StackOnly {
    return StackOnly(42);
}
```

## 应用场景

1. **RAII 资源管理**：
   - 确保资源在作用域结束时自动释放
   - 防止资源泄露

2. **性能优化**：
   - 避免堆分配的开销
   - 提高缓存命中率

3. **安全考虑**：
   - 防止内存泄露
   - 强制栈上生命周期管理

## 最佳实践

1. **明确声明意图**：
   - 在类文档中说明只能栈上分配
   - 使用 `= delete` 而不是私有化

2. **保持一致性**：
   - 如果类是只能栈上分配的，其成员也应考虑同样的限制
   - 在继承关系中保持一致

3. **异常安全**：
   - 确保析构函数不抛出异常
   - 正确处理拷贝/移动操作中的异常
