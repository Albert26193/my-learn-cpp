# 条款18：让接口容易被正确使用，不易被误用

## 问题：易于误用的接口

正常编译但易被误用的接口设计会导致用户错误。

### 不良设计示例

```cpp
// BadCase
class Date {
public:
    Date(int month, int day, int year): month(month), day(day), year(year) {};
    int month;
    int day;
    int year;
};
```

#### 使用情况

理想用户使用方式：
```cpp
Date d(12, 7, 2023);
```

实际用户常见错误：
```cpp
Date d(2023, 12, 7); // 日期顺序与定义不对
Date d(2, 30, 2023); // 2月没有30日
```

## 解决方案：参数使用包装类型

通过创建强类型来防止误用：

```cpp
// GoodCase
struct Month {
    explicit Month(int m) : val(m) {};
    int val;
};

struct Day {
    explicit Day(int d) : val(d) {};
    int val;
};

struct Year {
    explicit Year(int y) : val(y) {};
    int val;
};

class GoodDate {
public:
    GoodDate(const Month& month, const Day& day, const Year& year): 
        month(month.val), day(day.val), year(year.val) {};
    int month;
    int day;
    int year;
};
```

使用这种设计，用户必须显式指定每个参数的类型，从而避免参数顺序错误。

## 接口一致性问题

不同类使用不同的命名会导致混淆：

```cpp
// BadCase
class MyList {
    length() {}
}

class MyVector {
    size() {} // good in C++
}

class MyArray {
    int count;
}

// 用户困惑：应该用 length() 还是 size() 还是 count？
```

## 总结

- 好的接口很容易被正确使用，不容易被误用。应该在所有接口中努力达成这些性质。
- “促进正确使用” 的办法包括接口的一致性，以及与内置类型的行为兼容。
- “阻止误用” 的办法包括建立新类型、限制类型上的操作，束缚对象值，以及消除客户的资源管理责任。
- 接口行为应尽可能保持一致性，最好与 STL 标准库看齐。

## 代码运行命令

clang++ tutorial18.cpp -o app && ./app