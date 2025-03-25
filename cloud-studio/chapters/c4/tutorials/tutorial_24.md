# 条款 24. 如果需要所有参数都支持隐式转换，请把他写成普通函数

假设你定义了一个有理数类 `Rational`：

```cpp
class Rational {
public:
    Rational(int numerator = 0, int denominator = 1); 
    int numerator() const;   // accessors for numerator and
    int denominator() const; // denominator — see Item 22
private:
    // ...
};
```

## 成员函数实现的问题

现在需要给它写一个乘法运算符，一种常见的实现方式是：

```cpp
class Rational {
public:
    // ...
    const Rational operator*(const Rational& rhs) const;
};
```

看起来很正确。但是这种实现存在一个问题：

由于有理数可以和整数相乘，我们希望这种乘法支持 `int` 类型。因为构造函数没有声明 `explicit`，所以 `int` 可以隐式转为 `Rational` 类型。比如：

```cpp
Rational a(1, 2);
a * 2;  // 允许，因为 2 可以隐式转为 Rational
2 * a;  // 不允许！
```

## 为什么成员函数实现有限制？

第二个表达式为什么不允许？让我们分析一下：

1. `a * 2` 等价于 `a.operator*(2)`，其中 `2` 可以被隐式转换为 `Rational`
2. `2 * a` 等价于 `2.operator*(a)`——但数字 `2` 没有任何成员函数

也许你会想：类成员函数里其实有一个隐含的 `Rational` 类型的参数，它的值是 `*this`，那么 `2` 是否可以对应这个 `*this` 对象？

遗憾的是，这种想法是错误的。`*this` 这个隐含对象不会参与隐式类型转换。

## 解决方案：使用普通函数

所以回到标题的建议：应该把运算符重载实现为普通函数。

注意：这里的普通函数最好不是友元函数。在这个例子中，乘法运算符不需要直接访问内部成员变量（而是通过公有接口间接访问）。

一个参考实现如下：

```cpp
class Rational {
    // ... 不包含 operator*
};

// 非成员函数实现
const Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.numerator() * rhs.numerator(),
                   lhs.denominator() * rhs.denominator());
}

// 使用示例
Rational oneFourth(1, 4);
Rational result;
result = oneFourth * 2;  // 正常工作
result = 2 * oneFourth;  // 现在也可以工作了！
