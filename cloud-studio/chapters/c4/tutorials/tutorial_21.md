# 条款21：返回对象时，别妄想返回其reference

## 问题引入

虽然值传递会影响效率，但是千万不能在所有传递上都使用引用传递，因为这样可能会传递一些并不存在的对象。

## 示例分析

### 基础类定义

```cpp
class Rational {
public:
    // 条款24说明为什么这个构造函数不声明为explicit
    Rational(int numerator = 0, int denominator = 1);  

private:
    int n, d;
    // 条款3说明为什么返回类型是const
    friend const Rational operator*(const Rational& lhs, const Rational& rhs);
};
```

这个版本的 `operator*` 是以值传递方式返回其计算结果的。如果将其改为引用传递，则其一定指向某个已存在的 `Rational` 对象，内含两个 `Rational` 对象的乘积。我们不可能期望这样一个（内涵乘积的）`Rational` 对象在调用 `operator*` 之前就存在：

### 使用场景

```cpp
Rational a(1, 2);
Rational b(3, 5);
Rational c = a * b;  // 期望得到3/10
```

在这种情况下，期望原本就存在一个值为3/10的`Rational`对象是不合理的。如果`operator*`返回一个引用，它必须自己创建那个`Rational`对象。

### 错误示范

函数创建新对象有两种途径：stack空间或heap空间。

1. **在stack上创建（错误）**：
```cpp
// 危险：返回局部对象的引用
const Rational& operator*(const Rational& lhs, const Rational& rhs) {
    Rational result(lhs.n * rhs.n, lhs.d * rhs.d);
    return result;  // 返回一个已被销毁对象的引用！
}
```

2. **在heap上创建（错误）**：
```cpp
// 危险：内存泄漏
const Rational& operator*(const Rational& lhs, const Rational& rhs) {
    Rational* result = new Rational(lhs.n * rhs.n, lhs.d * rhs.d);
    return *result;  // 谁来delete这个对象？
}
```

### 正确实现

对于必须返回新对象的函数，正确的做法是：直接返回一个新对象。

```cpp
inline const Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.n * rhs.n, lhs.d * rhs.d);
}
```

虽然需要承担返回值的构造和析构成本，但这是为了获得正确行为而付出的必要代价。

## 关键原则

当在返回reference和object之间做选择时，应该选择能够产生正确行为的方案。

## 重要提醒

**永远不要返回**：
1. 指向局部栈对象的pointer或reference
2. 指向heap分配对象的reference
3. 指向局部static对象的pointer或reference

## 总结

- 返回reference可能导致悬垂引用或内存泄漏
- 虽然返回对象会有性能开销，但这是确保正确性的必要代价
- 在正确性和效率之间，应该优先考虑正确性
