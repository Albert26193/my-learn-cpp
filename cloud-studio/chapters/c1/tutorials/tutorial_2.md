# 条款02 尽量以const,enum,inline替换#define

> 原则:宁可以编译器替换预处理器

为什么？
- 让编译器看见符号，宏会让问题定位变得困难
- 宏有时候会产生与预期不符的效果

```cpp
#define CALL_WITH_MAX(a, b) a > b ? a : b

int main() {
    int num1 = 4;
    int num2 = 2;

    // Bad，num会自增两次
    int result = CALL_WITH_MAX(++num1, num2);

    std::cout << "result: " << result << std::endl;
    std::cout << "Expected num1: " << 5 << std::endl;
    std::cout << "Actual num1: " << num1 << std::endl;

    return 0;
}
```
