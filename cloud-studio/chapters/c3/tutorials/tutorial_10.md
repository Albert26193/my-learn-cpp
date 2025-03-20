# 10 operator= 返回 reference to *this

## 问题描述

向量类的设计需要考虑以下几点：

- 三个维度
- 完成四则运算、点乘叉乘

## 代码示例

```cpp
#include <iostream>

template <typename T>
class Vector3 {
public:
    T x, y, z;

    Vector3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

    Vector3& operator=(const Vector3& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3 operator+(const Vector3& other) const {
        return Vector3<T>(x + other.x, y + other.y, z + other.z);
    }

    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};

typedef Vector3<int> Vector3i;

// 注意：下面的代码似乎是不完整的模板声明
/*
templet
cross(vec,vec) {

}

corss(v1,v2);
*/

int main() {
    Vector3i v1(1, 2, 3);
    Vector3i v2(4, 5, 6);
    Vector3i v3(7, 8, 9);
    
    v1 = v2 += v3;  
    // v1 * v2;  // 注释掉的代码，因为没有定义乘法运算符
    v1.print();

    v3 = v1 + v2;
    v3.print();

    return 0;
}
```

## 讨论

### 什么时候写 `return *this;`
- 修改对象的状态 **且** 支持链式调用(赋值、加减、点乘)
- 例如：`operator=`、`operator+=`、`operator-=`等

### 什么时候不写 `return *this;`
- 需要新的对象 **或** 实现克隆功能
- 例如：`operator+`、`operator-`等

## 编译和运行

```bash
clang++ tutorial10.cpp -o app && ./app
```