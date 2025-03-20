# 条款13：以对象管理资源

## 问题：资源管理的挑战

### 使用裸指针 (raw pointer) 的问题

```cpp
class Investment { /* ... */ };

void func() {
    Investment* pInv = createInvestment();  // 调用factory函数
    // ...
    delete pInv;  // 释放pInv所指对象
}
```

如果在 `...` 区域内出现以下情况，会导致 `delete` 不被执行，最终出现**内存泄漏**：

- 一个过早的 `return` 语句
- 由于某个 `continue` 或 `goto` 语句过早退出
- 任何可能的异常抛出

即使谨慎编写程序可以防止这类错误，但随着时间推移，代码可能会被修改。一旦软件开始接受维护，可能会有人添加 `return` 或 `continue` 语句，而没有完全理解它对函数资源管理策略造成的后果。

- 解决方案：RAII (资源获取即初始化)

### 智能指针 auto_ptr (C++11已废弃)

标准程序库提供的 `auto_ptr` 正是针对这种问题而设计的。`auto_ptr` 是一个类指针对象（智能指针），其析构函数会自动对其所指对象调用 `delete`。

```cpp
class Investment { /* ... */ };

void func() {
    std::auto_ptr<Investment> pInv(createInvestment());
    // ...
    // 不需要手动delete，auto_ptr析构函数会自动处理
}
```

### auto_ptr 的特殊性质

由于 `auto_ptr` 被销毁时会自动删除它所指之物，所以一定要注意别让多个 `auto_ptr` 同时指向同一对象。

为了预防这个问题，`auto_ptr` 有一个不寻常的性质：若通过复制构造函数或赋值操作符复制它们，它们会变成 `null`，而复制所得的指针将取得资源的唯一拥有权！

```cpp
void func() {
    std::auto_ptr<Investment> pInv1(createInvestment()); // pInv1指向createInvestment返回物
    
    std::auto_ptr<Investment> pInv2(pInv1); // pInv2指向对象，pInv1被设为null
    
    pInv1 = pInv2; // pInv1指向对象，pInv2被设为null
}
```

### 更好的替代方案：shared_ptr

```cpp
void func() {
    std::shared_ptr<Investment> pInv1(createInvestment());
    
    std::shared_ptr<Investment> pInv2(pInv1); // 两个指针共享同一对象
    
    pInv1 = pInv2; // 赋值操作不改变指针状态
    
    // ...
    // 当最后一个shared_ptr离开作用域，对象自动被删除
}
```

## RAII 的核心思想

RAII（Resource Acquisition Is Initialization）有两个关键点：

1. **获得资源后立刻放进管理对象内**
2. **管理对象运用析构函数确保资源被释放**

## 总结

- **防止资源泄漏**：使用RAII对象，它们在构造函数中获得资源并在析构函数中释放资源。
- **简化资源管理**：通过将资源的获取、释放与对象的生命周期绑定，可以简化资源管理的代码，减少错误和遗漏。
- **异常安全**：当异常发生时，RAII对象的析构函数会自动执行，确保资源得到正确释放，避免资源泄漏。
- **可读性和可维护性**：RAII范式使得代码更具有可读性和可维护性，因为资源管理的逻辑与对象的生命周期紧密相关。
- **避免资源泄漏**：RAII对象通过自动管理资源的分配和释放，有效避免了资源泄漏的问题。

### 常用的RAII对象
- `std::shared_ptr`：共享所有权的智能指针
- `std::unique_ptr`：独占所有权的智能指针
- `std::auto_ptr`：已废弃，不推荐使用

## 代码运行命令
```bash
clang++ tutorial13.cpp -o app && ./app