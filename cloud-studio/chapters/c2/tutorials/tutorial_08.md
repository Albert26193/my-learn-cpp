# 08 析构函数中的异常处理

## 问题引入：析构函数抛出异常的危险

虽然 C++ 语言层面并不禁止析构函数抛出异常，但这是一种非常危险的行为：
- 可能导致程序过早终止
- 可能产生未定义行为
- 在对象数组或容器析构时特别危险

## 示例：危险的析构函数设计

```cpp
class A {
public:
    static uint32_t count;
    
    A() {
        id = ++count;
    }
    
    ~A() {
        if (id == 5) {
            throw "id is five";  // 危险：析构函数抛出异常！
        }
    }
    
private:
    int32_t id;
};

uint32_t A::count = 0;

int main() {
    try {
        auto vec = std::vector<A>(10);
        // vec 析构时会抛出异常，因为第5个元素的 id == 5
    } catch (const char* msg) {
        std::cerr << "捕获异常: " << msg << std::endl;
        return 1;
    }
    return 0;
}
```

## 正确的异常处理方式

### 1. 在析构函数中捕获异常

```cpp
class A {
public:
    ~A() {
        try {
            // 可能抛出异常的清理操作
            cleanup();
        } catch (...) {
            // 记录错误
            std::cerr << "析构函数捕获到异常" << std::endl;
            
            // 选择1：吞下异常（不推荐，但比抛出要好）
            // 选择2：调用 std::abort()
            std::abort();
        }
    }
private:
    void cleanup() {
        // 清理操作
    }
};
```

### 2. 提供独立的清理函数

```cpp
class Connection {
public:
    void close() {  // 普通函数可以抛出异常
        if (!closeConnection()) {
            throw ConnectionError("关闭连接失败");
        }
    }
    
    ~Connection() {
        try {
            if (isOpen()) {
                closeConnection();  // 析构函数中静默处理错误
            }
        } catch (...) {
            // 记录错误但不抛出
            std::cerr << "关闭连接时发生错误" << std::endl;
        }
    }
};
```

## 最佳实践

1. **析构函数绝不抛出异常**
   - 要么捕获并处理异常
   - 要么捕获并终止程序
   - 绝不允许异常逃逸

2. **如果需要异常处理**
   - 提供独立的清理函数
   - 让用户选择是否处理异常
   - 在析构函数中提供静默的清理

3. **设计建议**
   - 将可能抛出异常的操作从析构函数中分离
   - 提供显式的清理接口
   - 在析构函数中提供基本的资源清理保证

## 编译和运行

```bash
clang++ -std=c++11 -Wall -Wextra tutorial_08.cpp -o app && ./app