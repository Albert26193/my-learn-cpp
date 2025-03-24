# C++ 头文件保护符（Header Guards）

## 基本语法

```cpp
#ifndef __SOME_UNIQUE_NAME_H
#define __SOME_UNIQUE_NAME_H

// 头文件内容

#endif  // __SOME_UNIQUE_NAME_H
```

## 工作原理

1. **首次包含**：
   ```cpp
   #ifndef __NODE__H       // 检查 __NODE__H 是否已定义
   #define __NODE__H       // 如果未定义，则定义它
   // 头文件内容被包含
   #endif
   ```

2. **重复包含**：
   ```cpp
   #ifndef __NODE__H       // 检查 __NODE__H 是否已定义
   // 由于已经定义，跳过整个内容
   #endif
   ```

## 为什么需要头文件保护符

### 1. 防止重复定义
```cpp
// a.h
class A {};

// b.h
#include "a.h"

// main.cpp
#include "a.h"
#include "b.h"  // 如果没有头文件保护符，A 类会被定义两次
```

### 2. 解决循环依赖
```cpp
// foo.h
class Bar;  // 前向声明
class Foo {
    Bar* bar;
};

// bar.h
class Foo;  // 前向声明
class Bar {
    Foo* foo;
};
```

## 命名约定

1. **传统风格**：
```cpp
#ifndef __NODE__H
#define __NODE__H
```

2. **现代风格**（推荐）：
```cpp
#ifndef PROJECT_PATH_FILENAME_H
#define PROJECT_PATH_FILENAME_H
```

3. **项目专用前缀**：
```cpp
#ifndef MYPROJECT_NODE_H
#define MYPROJECT_NODE_H
```

## 替代方案

1. **#pragma once**：
```cpp
#pragma once
// 头文件内容
```
- 更简洁
- 编译器支持良好
- 但不是 C++ 标准

## 最佳实践

1. **选择唯一的标识符**：
   - 使用项目名称作为前缀
   - 包含文件路径信息
   - 避免以下划线开头的名称（保留给实现）

2. **保持一致性**：
   - 在项目中统一使用一种风格
   - 如果使用 `#pragma once`，就全部使用它

3. **注释结尾**：
```cpp
#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

// 头文件内容

#endif  // PROJECT_NODE_H  // 添加注释便于阅读
```

4. **避免常见错误**：
   - 不要忘记 `#endif`
   - 确保宏名称在项目中唯一
   - 不要在头文件保护符之前放置代码
