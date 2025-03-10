# C++ 的四大关系语言
## 1. C 语言
- C 是 C++ 的基础。包括：（`blocks`/`statements`/`preprocessor`/`built-in data types`/`arrays`/`pointers`）
- 但是 C 没有*模板*、没有*异常*、没有*重载*
- C++ 是 C 更高级解法

## 2. Object -Oriented C++ 面向对象编程
- 封装
- 继承
- 多态

## 3. Template C++ 泛型编程
- 模板威力巨大
- 是区分 C++ 编码水平的重要标尺

## 4. STL 标准库
- 容器
- 迭代器
- 算法
- 问题注意

1. 内置类型，`pass-by-value` 比 `pass-by-reference` 更加高效

```cpp
// Good
void set_built_in_type(int value) {

}

// Bad
void set_built_in_type(const int &value) {

}
```

2. 自定义类型，`pass-by-reference-const` 更好

```cpp
class UserDefinedType {
    public:
    int value;
    UserDefinedType(const UserDefinedType &obj){
        value = obj.value;
    }
};

// Good
void set_user_defined_type(const UserDefinedType &value) {

}

// Bad，会调用构造函数
void set_user_defined_type(UserDefinedType value) {

}

// Bad
void set_user_defined_type(UserDefinedType &value) {

}
```

3. 代码运行命令

```bash
clang++ tutorial1.cpp -o app && ./app
```