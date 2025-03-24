# C++ 的构造函数类型和赋值运算符最佳实践

## 构造函数类型：

1. **默认构造函数**：无参数构造函数
   ```cpp
   MyClass();
   ```
   
2. **有参构造函数**：有参数构造函数
   ```cpp
   MyClass(int value);
   ```
   
3. **拷贝构造函数**：拷贝构造函数
   ```cpp
   MyClass(const MyClass& other);
   ```
   
4. **移动构造函数** (C++11)：移动构造函数
   ```cpp
   MyClass(MyClass&& other) noexcept;
   ```
   
5. **委托构造函数**：委托构造函数
   ```cpp
   MyClass(int value) : MyClass(value, 0) {}
   MyClass(int x, int y) : data_x(x), data_y(y) {}
   ```
   
6. **转换构造函数**：转换构造函数
   ```cpp
   explicit MyClass(const std::string& str);
   ```
   
7. **初始化列表构造函数** (C++11)：初始化列表构造函数
   ```cpp
   MyClass(std::initializer_list<int> values);
   ```

## 赋值运算符：

1. **拷贝赋值运算符**：拷贝赋值运算符
   ```cpp
   auto operator=(const MyClass& other) -> MyClass&;
   ```
   
2. **移动赋值运算符** (C++11)：移动赋值运算符
   ```cpp
   auto operator=(MyClass&& other) noexcept -> MyClass&;
   ```

**最佳实践：**
- 在赋值运算符中检查自赋值 (`if (this != &other)`)
- 赋值运算符应返回 `*this` 的引用以支持链式赋值
- 使用 `= delete` 禁用不需要的拷贝/赋值操作
- 考虑禁用危险操作：`operator new = delete;`