# 条款17：以独立语句将 newed 对象置入智能指针

## 问题描述

观察以下代码：

```cpp
class Widget {
    
};

int main() {
    int priority();
    void processWidget(std::shared_ptr<Widget> pw, int priority);

    // 无法通过编译
    processWidget(new Widget, priority());

    // 虽然能通过编译，但有可能造成内存泄漏
    processWidget(std::shared_ptr<Widget>(new Widget), priority());
}
```

## 问题分析

编译器产出 `processWidget` 调用码之前，必须首先核算即将被传递的各个实参：

- 上述第二实参只是一个单纯对 `priority` 函数的调用
- 第一实参由两部分组成：
  1. 执行 `new Widget` 表达式
  2. 调用 `shared_ptr` 构造函数

### 最终调用顺序可能是：

1. 调用 `priority()`
2. 执行 `new Widget` 表达式
3. 调用 `shared_ptr` 构造函数

**问题**：万一 `priority()` 调用抛出异常，`new Widget` 返回的指针将会遗失，因为它未被置入 `shared_ptr` 内，从而导致内存泄漏。

## 解决方案

```cpp
class Widget {
    
};

int main() {
    int priority();
    void processWidget(std::shared_ptr<Widget> pw, int priority);

    // 正确做法：先创建智能指针，再传入函数
    std::shared_ptr<Widget> pw(new Widget);
    processWidget(pw, priority());
}
```

## 总结

- **以独立语句将 newed 对象置入智能指针内**。否则，一旦异常被抛出，有可能导致难以察觉的资源泄漏。

## 代码运行命令

```bash
clang++ tutorial17.cpp -o app && ./app
```

## 现代 C++ 的最佳实践

在 C++11 及更高版本中，推荐使用 `std::make_shared` 来创建智能指针：

```cpp
// 更好的方式
std::shared_ptr<Widget> pw = std::make_shared<Widget>();
processWidget(pw, priority());
```

这样不仅能避免内存泄漏问题，还能提高性能（只需一次内存分配而非两次）。