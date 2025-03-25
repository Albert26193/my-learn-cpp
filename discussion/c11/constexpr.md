# constexpr 关键字详解

## 设计目的

constexpr 是 C++11 引入的关键字，主要解决以下问题：
1. 在编译期进行计算，提高运行时性能
2. 保证编译期常量的类型安全
3. 支持更复杂的编译期计算
4. 减少运行时开销

## 使用要点

1. 基本语法：
```cpp
constexpr int square(int n) { return n * n; }
constexpr int value = square(5);  // 编译期计算
```

2. 函数要求：
- 必须有返回值（void 除外，C++14 开始支持）
- 函数体只能包含：
  * return 语句
  * 类型定义
  * 静态断言
  * using 声明
- 不能有：
  * try-catch
  * goto
  * 非字面值类型的变量

3. 变量要求：
```cpp
constexpr int size = 100;          // 编译期常量
constexpr double pi = 3.14159;     // 字面值类型
constexpr auto ptr = &global_var;  // 地址必须固定
```

4. 类的支持：
```cpp
class Point {
    constexpr Point(int x, int y) : x_(x), y_(y) {}
    constexpr int getX() const { return x_; }
    constexpr int getY() const { return y_; }
private:
    int x_, y_;
};
```

## 底层实现原理

1. 编译期求值：
- 编译器在编译时执行 constexpr 表达式
- 结果直接编码到目标文件中
- 避免运行时计算开销

2. 实现机制：
- 编译器维护常量表
- 表达式树优化
- 编译期类型检查
- 常量折叠（Constant Folding）

3. 两阶段行为：
```cpp
constexpr int getValue(int n) {
    return n * 2;
}

// 编译期计算
constexpr int a = getValue(5);  // 编译期求值

// 运行时计算
int x = 10;
int b = getValue(x);  // 运行时求值
```

## 最佳实践

1. 使用场景：
- 数组大小声明
- 模板参数
- 枚举值计算
- 编译期数据结构和算法

2. 注意事项：
- 不要过度使用，可能增加编译时间
- 保持函数简单，便于编译器优化
- 考虑可维护性和调试难度
- 与 consteval（C++20）区分使用

3. 性能考虑：
- 编译期计算减少运行时开销
- 但可能增加编译时间和二进制大小
- 需要在编译时间和运行性能间权衡
