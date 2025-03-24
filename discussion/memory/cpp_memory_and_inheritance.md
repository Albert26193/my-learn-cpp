# C++ 内存模型与继承

## 1. C++ 程序的内存布局

```mermaid
graph TB
    subgraph Memory Layout
        direction TB
        A[栈区 Stack] -->|向下增长| B[未分配区域]
        B -->|向上增长| C[堆区 Heap]
        C --> D[数据段 Data Segment]
        D --> E[代码段 Text Segment]
        
        style A fill:#f9f,stroke:#333,stroke-width:2px
        style C fill:#bbf,stroke:#333,stroke-width:2px
        style D fill:#dfd,stroke:#333,stroke-width:2px
        style E fill:#fdd,stroke:#333,stroke-width:2px
    end
```

### 1.1 各个段的特点

1. **代码段（Text Segment）**：
   - 存储：程序的机器码、类的成员函数实现
   - 特点：只读、所有进程共享
   - 位置：最低地址

2. **数据段（Data Segment）**：
   - 已初始化数据段：全局变量、静态变量、常量
   - 未初始化数据段（BSS）：未初始化的全局/静态变量
   - 特点：可读写、进程独立

3. **堆区（Heap）**：
   - 动态分配的内存（new/malloc）
   - 从低地址向高地址增长
   - 需要手动管理（delete/free）

4. **栈区（Stack）**：
   - 局部变量、函数参数
   - 从高地址向低地址增长
   - 自动管理

## 2. 类的内存布局

```mermaid
classDiagram
    class BaseNode {
        -uint64_t id
        +BaseNode(uint64_t)
        +~BaseNode()
        +get_id() uint64_t
    }
    
    class RectangleNode {
        -uint32_t* width
        -uint32_t* height
        +RectangleNode(uint64_t)
        +~RectangleNode()
        +get_area() uint32_t
    }
    
    BaseNode <|-- RectangleNode
```

### 2.1 对象内存布局

```mermaid
graph TD
    subgraph "对象内存布局"
        A[vptr 虚函数表指针<br>8字节] -->|如果有虚函数| B[数据成员]
        B -->|对齐要求| C[填充字节]
    end
```

### 2.2 类大小计算

影响类大小的因素：
```mermaid
mindmap
    root((类大小))
        数据成员
            基本类型大小
            对象成员大小
        虚函数表指针
            有虚函数时 +8字节
        内存对齐
            基本对齐
            整体对齐
        空类处理
            至少1字节
```

## 3. 虚析构函数问题

```mermaid
sequenceDiagram
    participant Main
    participant Base as BaseNode
    participant Derived as RectangleNode
    
    Note over Main: 正确的删除顺序
    Main->>Derived: delete derived_ptr
    Derived->>Derived: ~RectangleNode()
    Derived->>Base: ~BaseNode()
    
    Note over Main: 错误的删除顺序
    Main->>Base: delete base_ptr
    Base->>Base: ~BaseNode()
    Note over Derived: 析构函数未被调用!
```

### 3.1 非虚析构函数的问题

```mermaid
graph TD
    A[通过基类指针删除] -->|非虚析构函数| B[只调用基类析构函数]
    B --> C[派生类析构函数未调用]
    C --> D[资源泄漏]
    C --> E[未定义行为]
```

### 3.2 解决方案

```mermaid
graph LR
    A[基类析构函数] --> B{需要多态删除?}
    B -->|是| C[虚析构函数]
    B -->|否| D[受保护非虚析构函数]
    C --> E[支持多态,但有虚表开销]
    D --> F[编译期防错,无虚表开销]
```

## 4. 编程最佳实践

1. **析构函数设计**：
   - 基类需要多态删除时使用虚析构函数
   - 不需要多态删除时使用受保护非虚析构函数
   - 注意资源管理和异常安全

2. **内存管理**：
   - 优先使用智能指针
   - 遵循 RAII 原则
   - 注意内存对齐要求

3. **类的设计**：
   - 合理组织数据成员以优化内存布局
   - 注意虚函数的开销
   - 明确所有权语义
