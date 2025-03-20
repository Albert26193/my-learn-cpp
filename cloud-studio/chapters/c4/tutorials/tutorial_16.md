# 条款16：成对使用 new 和 delete 时要采取相同形式

## 问题描述

以下代码有什么错？

```cpp
std::string* strArr = new std::string[100];
delete strArr;
```

- 看起来都井然有序。使用了new，也搭配了对应的 delete。

- 但有某样东西完全错误: **你的程序行为不明确 (未有定义)**

- strArr 中所含的100个string对象中的99个不太可能被适当删除。

## new 和 delete 的作用原理

### 当调用new的时候发生了什么？

1. 内存被分配出来
2. 针对此内存会有一个或多个构造函数被调用

### 当调用delete的时候发生了什么？

1. 针对此内存会有一个(或更多)析构函数被调用
2. 然后内存才被释放

delete的最大问题在于: 即将被删除的内存之内究竟存有多少对象？这个问题的答案决定了有多少个析构函数被必须调用起来。

## 正确的做法

```cpp
// 正确的做法
std::string* strArr1 = new std::string;       // 单个对象

std::string* strArr2 = new std::string[100];  // 数组

delete strArr1;      // 删除单个对象
delete[] strArr2;    // 删除数组
```

## 总结

1. 如果你在new表达式中使用[]，必须在相应的delete表达式中也使用[]。

2. 如果你在new表达式中不使用[]，一定不要在相应的delete表达式中使用[]。

## 代码运行命令

```bash
clang++ tutorial16.cpp -o app && ./app
```