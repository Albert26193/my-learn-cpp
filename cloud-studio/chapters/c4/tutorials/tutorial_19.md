# 条款 19：设计 class 犹如设计 type

C++ 就像在其他 OOP 语言一样，当你定义一个新 class，也就定义了一个新 type。这意味着你并不只是 class 设计者，还是 type 设计者。

那么，如何设计高效的 class 呢？首先必须了解你面对的问题。几乎每一个 class 都要求你面对下面的提问，而你的回答往往导致你的设计规范：

## 设计 class 时需要考虑的问题

### 1. 新 type 的对象应该如何被创建和销毁
   - 这会影响到你的 class 的构造函数和析构函数以及内存分配函数和释放函数（`new` 和 `delete`）的设计。
  class 构建出来的就是一个对象，对象的生成和销毁是需要特别关注的环节，存在多种风险行为：
  
  **潜在风险行为：**
  - 内存泄漏：忘记释放动态分配的内存
  - 重复删除：同一指针被多次删除导致未定义行为
  - 不匹配的 new/delete：使用 `new[]` 分配但用 `delete` 释放（应使用 `delete[]`）
  - 异常安全问题：构造过程中发生异常导致资源未正确释放
  - 虚析构函数缺失：通过基类指针删除派生类对象时，派生类析构函数不被调用
  
  **对象创建的方法：**
  - 栈上自动创建：`MyClass obj;` 或 `MyClass obj{args};`（推荐使用花括号初始化）
  - 动态分配单个对象：`MyClass* ptr = new MyClass();`
  - 动态分配对象数组：`MyClass* arr = new MyClass[10];`
  - 智能指针创建（推荐）：
    - `auto ptr = std::make_unique<MyClass>(args);` (C++14，独占所有权)
    - `auto ptr = std::make_shared<MyClass>(args);` (C++11，共享所有权)
  - 定位 new（placement new）：`new (address) MyClass();`（在预分配内存上构造对象）
  - 容器内构造：`emplace_back()`, `emplace()`（直接在容器内存中构造，避免额外拷贝）
  
  **对象销毁的方法：**
  - 栈对象自动销毁：作用域结束时自动调用析构函数
  - 动态对象显式销毁：`delete ptr;` 或 `delete[] arr;`
  - 智能指针自动管理：离开作用域或引用计数为零时自动销毁
  - 显式调用析构函数：`obj.~MyClass();`（仅用于特殊场景如placement new）
  
  **最佳实践：**
  - 优先使用智能指针而非原始指针管理资源
  - 确保 new/delete 和 new[]/delete[] 正确配对
  - 为多态基类提供虚析构函数
  - 使用 RAII（资源获取即初始化）原则设计类
  - 考虑禁用危险操作：`operator new = delete;`

### 2. 对象的初始化和对象的赋值有什么样的差别
   - 这个答案决定你的构造函数和赋值运算符的行为，以及之间的差别。不要混淆“初始化”和“赋值”。

   **对象初始化与赋值的区别：**
   - **初始化**：在对象创建时为其赋予初始值，通常通过构造函数实现。
   - **赋值**：在对象已存在时为其重新赋值，通常通过赋值运算符实现。

   **示例：**
   ```cpp
   class MyClass {
   public:
       // 构造函数（初始化）
       MyClass(int value) : data(value) {}
       
       // 赋值运算符（赋值）
       auto operator=(const MyClass& other) -> MyClass& {
           data = other.data;
           return *this;
       }
   
   private:
       int data;
   };
   
   // 初始化示例
   MyClass obj(10);  // 调用构造函数，初始化对象
   
   // 赋值示例
   obj = MyClass(20);  // 调用赋值运算符，重新赋值对象
   ```

   **C++ 的构造函数类型和赋值运算符最佳实践：**

   **构造函数类型：**

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
   
   **赋值运算符：**

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

### 3. 新 type 的对象如果被 passed by value，意味着什么
   - 拷贝构造函数用来定义一个 type 的 pass-by-value 怎么实现。

   **拷贝构造函数的作用：**
   - 当对象被传递给函数时，会调用拷贝构造函数创建一个临时副本。
   - 拷贝构造函数负责初始化新对象，使其成为原始对象的副本。

   **示例：**
   ```cpp
   class MyClass {
   public:
       // 拷贝构造函数
       MyClass(const MyClass& other) : data(new int(*other.data)) {
           std::cout << "拷贝构造函数被调用" << std::endl;
       }
       
       ~MyClass() { delete data; }
       
       // 其他成员函数...
   private:
       int* data;
   };
   
   // 传递对象给函数
   void processObject(MyClass obj) {  // 注意，这里会调用拷贝构造函数
       // 对象处理...
   }  // 函数结束，临时副本被销毁
   
   MyClass original;
   processObject(original);  // 调用拷贝构造函数，创建临时副本
   ```

   **最佳实践：**
   - 对于大型对象，应使用引用或指针传递，而不是传值。
   - 如果必须传值，应使用移动构造函数和移动赋值运算符来减少不必要的拷贝。
   - 使用 `= delete` 禁用不需要的拷贝/赋值操作。

### 4. 什么是新 type 的“合法值”？
   - 对 class 的成员变量而言，通常只有某些数值集是有效的。那些数值集决定了你对 class 必须维护的约束条件，也就决定了你的成员函数必须进行的错误检查工作。它也影响函数抛出的异常。

   **合法值的含义：**
   - 合法值是指对象可以拥有的有效状态或值。
   - 合法值的定义通常依赖于具体的应用场景和业务逻辑。

   **示例：**
   ```cpp
   class Date {
   public:
       Date(int year, int month, int day) {
           // 检查合法值
           if (month < 1 || month > 12) {
               throw std::invalid_argument("月份必须在 1-12 之间");
           }
           
           if (day < 1 || day > daysInMonth(year, month)) {
               throw std::invalid_argument("日期不合法");
           }
           
           this->year = year;
           this->month = month;
           this->day = day;
       }
       
       // 其他成员函数...
   
   private:
       int year;
       int month;
       int day;
       
       // 返回月份的天数
       int daysInMonth(int y, int m) const {
           // 实现...
       }
   };
   ```

   **验证方法：**
   1. **构造函数验证**：在构造函数中检查合法值。
   2. ** setter 函数验证**：在 setter 函数中检查合法值。
   3. **成员函数验证**：在成员函数中检查合法值。
   4. **异常处理**：使用异常处理机制来处理非法值。

   **最佳实践：**
   - 在构造函数和 setter 函数中验证合法值。
   - 使用异常处理机制来处理非法值。
   - 文档化合法值的定义和验证规则。

### 5. 你的新 type 需要配合某个继承图系吗？
   - 如果你继承自某些已有的 class，你就受到那些 class 的设计的舒服，特别是受到它们的函数是 virtual 或 non-virtual 的影响（见条款 34 和 36）。如果你允许其他 class 继承你的 class，那么会影响你所声明的函数，尤其是析构函数是否为 virtual。

   **继承设计的考虑：**
   
   **1. 继承的类型：**
   - **公有继承**：子类继承父类的所有成员，包括公有和私有成员。
     ```cpp
     class Base {
     public:
         void foo() { }
     };
     
     class Derived : public Base {
     public:
         void bar() { foo(); }  // 访问父类的公有成员
     };
     ```
   
   - **私有继承**：子类继承父类的所有成员，但所有成员都变为私有。
     ```cpp
     class Base {
     public:
         void foo() { }
     };
     
     class Derived : private Base {
     public:
         void bar() { foo(); }  // 访问父类的私有成员
     };
     ```
   
   - **保护继承**：子类继承父类的所有成员，公有成员变为保护成员，私有成员保持不变。
     ```cpp
     class Base {
     public:
         void foo() { }
     };
     
     class Derived : protected Base {
     public:
         void bar() { foo(); }  // 访问父类的保护成员
     };
     ```
   
   **2. 虚函数和多态：**
   - **虚函数**：在基类中声明为 virtual 的函数，可以在派生类中被重写。
     ```cpp
     class Base {
     public:
         virtual void foo() { }
     };
     
     class Derived : public Base {
     public:
         void foo() override { }  // 重写基类的虚函数
     };
     ```
   
   - **多态**：通过基类指针或引用访问派生类对象时，会调用派生类的虚函数。
     ```cpp
     Base* ptr = new Derived();
     ptr->foo();  // 调用派生类的虚函数
     ```
   
   **3. 纯虚函数和抽象类：**
   - **纯虚函数**：在基类中声明为纯虚函数的函数，必须在派生类中被实现。
     ```cpp
     class Base {
     public:
         virtual void foo() = 0;  // 纯虚函数
     };
     
     class Derived : public Base {
     public:
         void foo() override { }  // 实现纯虚函数
     };
     ```
   
   - **抽象类**：包含纯虚函数的类，不能被实例化。
     ```cpp
     class Base {
     public:
         virtual void foo() = 0;  // 纯虚函数
     };
     
     // 不能实例化抽象类
     // Base obj;
     ```
   
   **最佳实践：**
   - 使用公有继承来实现"is-a"关系。
   - 使用私有继承来实现"has-a"关系。
   - 使用保护继承来实现"has-a"关系，并提供访问保护成员的接口。
   - 使用虚函数和多态来实现运行时多态。
   - 使用纯虚函数和抽象类来定义接口和规范。

### 6. 你的新 type 需要什么样的转换？
   - 如果你希望允许 T1 类型隐式转换为 T2 类型，就必须在 class T1 内写一个类型转换函数或者 class T2 内写一个可被单一实参调用的构造函数。

### 7. 什么样的操作符和函数对此新 type 而言是合理的
   - 这个问题的答案决定你将为你的 class 声明哪些函数。其中那些是成员函数，哪些不是。

### 8. **什么样的标准函数应该驳回
   - 那些被你必须声明为 private 的。

### 9. 谁该取用新 type 的成员
   - 这个问题可以帮你决定哪个成员为 public，哪个为 protected，哪个为 private。它也帮助你决定哪一个 class 和/或 function 应该是友元。

### 10. 什么是新 type 的“未声明接口”？
    - 它对效率、异常安全性（见条款 29）以及资源运用（例如多任务锁定和动态内存）提供何种保证？你在这些方面提供的保证将为你的 class 实现代码加上相应的约束条件。

### 11. 你的新 type 有多么一般化？
    - 或许你起始并非定义一个新 type，而是顶一个一整个 type 家族。果真如此你就不该定义一个新 class，而应该定义一个新 class template。

### 12. 你真的需要一个新 type 吗？
    - 如果只是i当以新的 derived class 以便为既有的 class 添加技能，那么说不定单纯定义一或多个 non-member 函数或 templates，更能够达到目标。

请记住：

-  Class 的设计就是 type 的设计。在定义一个新 type 之前，请确定你已经考虑过本条款覆盖的所有讨论主题。
