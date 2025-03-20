# 11 operator= 处理自我赋值

## 问题讨论

在设计类的赋值运算符时，我们需要考虑自我赋值的问题。自我赋值是指将一个对象赋值给它自己，例如：

```cpp
array[i] = array[j];  // 当 i == j 时发生自我赋值
*a = *b;              // 当 a 和 b 指向同一个对象时发生自我赋值
```

在继承关系中，也可能发生自我赋值的问题：

```cpp
Base* base;
Derived* der;
// 当 base 和 der 指向同一个对象时可能发生自我赋值
```

## 代码示例

```cpp
#include <iostream>
#include <vector>

template <typename T>
class Vector3 {
public:
    T x, y, z;

    Vector3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z), lists(new T[3]{x,y,z}) {}

    ~Vector3() {
        delete[] lists;
    }

    Vector3& operator=(const Vector3& other) {
        // 这里没有处理自我赋值的问题，可能导致问题
        delete[] lists;
        lists = new T[3];
        for (size_t i = 0; i < 3; ++i) {
            lists[i] = other.lists[i]; 
            std::cout << lists[i] << std::endl;
        }
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    T* getLists() {
        return lists;
    }

    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }
private:
    T* lists;
};

## 讨论

### 何时需要检查 `this != &other`
- 当类中有指针成员，需要手动分配和释放内存时
- 这种情况下，自我赋值可能导致问题

### 何时不需要检查 `this != &other`
- 当类中只有基本类型的成员变量的操作时
- 当使用了拷贝赋值（copy-on-write）或其它方式处理自我赋值的方法时

### 处理自我赋值的方法
1. **直接检查**：在赋值运算符开始时检查是否为自我赋值
   ```cpp
   if (this != &other) {
       // 处理赋值
   }
   return *this;
   ```

2. **临时对象交换**：创建一个临时对象，交换内容
   ```cpp
   Vector3 temp(other);  // 创建临时对象
   std::swap(*this, temp);  // 交换内容
   return *this;
   ```

3. **异常安全的方法**：分配新内存，复制内容，交换指针
   ```cpp
   T* new_lists = new T[3];
   for (size_t i = 0; i < 3; ++i) {
       new_lists[i] = other.lists[i];
   }
   delete[] lists;
   lists = new_lists;
   ```

## 编译和运行

```bash
clang++ tutorial11.cpp -o app && ./app