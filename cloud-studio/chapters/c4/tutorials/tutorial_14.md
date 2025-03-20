条款14：在资源管理类中小心 coping 行为

运行
需要注意的拷贝构造函数和拷贝赋值操作符的行为，以避免资源的重复释放或者悬挂指针等问题。

// BadCase

class BadCase {
public:
    BadCase(size_t size) {
        data_ = new int[size];
    }

    // 拷贝构造函数
    BadCase(const BadCase& other) {
        // 错误：直接拷贝原始指针，导致两个对象指向同一块内存
        data_ = other.data_;
    }

    // 拷贝赋值操作符
    BadCase& operator=(const BadCase& other) {
        // 错误：直接拷贝原始指针，导致两个对象指向同一块内存
        data_ = other.data_;
        return *this;
    }

    ~BadCase() {
        delete[] data_;
    }

private:
    int* data_;
};

int main() {
    BadCase m1(10);
    BadCase m2(m1); // 调用拷贝构造函数，m1 和 m2 指向同一块内存

    // 当 m2 被销毁时，会释放内存，但 m1 仍然持有已经被释放的内存的指针
    // 当 m1 被销毁时，会再次尝试释放同一块内存，导致未定义行为
    return 0;
}
// GoodCase

class GoodCase {
public:
    GoodCase(size_t size) : size_(size) {
        data_ = new int[size_];
    }

    // 拷贝构造函数
    GoodCase(const GoodCase& other) : size_(other.size_) {
        data_ = new int[size_];
        // 深拷贝：分配新的内存并复制数据
        std::copy(other.data_, other.data_ + size_, data_); 
    }

    // 拷贝赋值操作符
    GoodCase& operator=(const GoodCase& other) {
        if (this == &other) {
            // 自赋值检查
            return *this;
        }
        
        // 释放原有内存
        delete[] data_;

        size_ = other.size_;
        data_ = new int[size_];
        // 深拷贝：分配新的内存并复制数据
        std::copy(other.data_, other.data_ + size_, data_);

        return *this;
    }

    ~GoodCase() {
        delete[] data_;
    }

private:
    int* data_;
    size_t size_;
};

int main() {
    GoodCase m1(10);
    GoodCase m2(m1); // 调用拷贝构造函数，为 mm2 分配新的内存并复制 mm1 的数据

    // 当 mm1 和 mm2 被销毁时，各自释放各自的内存，没有悬挂指针和重复释放内存的问题
    return 0;
}
代码运行命令
clang++ tutorial14.cpp -o app && ./app