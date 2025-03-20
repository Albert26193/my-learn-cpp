# 条款12：复制对象勿忘其每一个成分

### 情况 1：拷贝构造函数

复制每一个成员变量

```cpp
class Example {
public:
    int a;
    double b;

    Example(int _a, double _b) : a(_a), b(_b) {}

    Example(const Example& other) : a(other.a), b(other.b) {}

    Example& operator=(const Example& other) {
        if (this != &other) {
            a = other.a;
            b = other.b;
        }
        return *this;
    }
};
```

### 情况 2：包含动态内存的类

数组需要人为处理，不能直接赋值；而 vector 会自动处理 copy constructor 和 copy assignment operator：

```cpp
class ArrayExample {
public:
    int* data;
    int size;

    ArrayExample(int size) : size(size), data(new int[size]) {}

    ~ArrayExample() { delete[] data; }

    ArrayExample(const ArrayExample& other) : size(other.size), data(new int[other.size]) {
        for (int i = 0; i < other.size; ++i) {
            data[i] = other.data[i];
        }
    }

    ArrayExample& operator=(const ArrayExample& other) {
        if (this != &other) {
            delete[] data;
            data = new int[other.size];
            size = other.size;
            for (int i = 0; i < other.size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    void print() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i] << ' ';
        }
        std::cout << '\n';
    }
};
```

### 情况 3：派生类

派生类必须确保调用基类的拷贝操作：

```cpp
class DerivedArrayExample : public ArrayExample {
public:
    int extra;

    DerivedArrayExample(size_t size, int extra) : ArrayExample(size), extra(extra) {}

    DerivedArrayExample& operator=(const DerivedArrayExample& other) {
        ArrayExample::operator=(other);   
        extra = other.extra;
        return *this;
    }
};
```

## 关键点

1. **基本原则**：复制对象时必须复制所有成员变量
2. **动态内存**：包含动态内存的类需要深拷贝
3. **派生类**：派生类必须调用基类的拷贝操作
4. **资源管理**：考虑使用智能指针来简化资源管理

## 编译和运行

```bash
clang++ tutorial12.cpp -o app && ./app