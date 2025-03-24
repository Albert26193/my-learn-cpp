
/**
 * @file tutorial_19.cpp
 * @brief 演示C++类设计的关键概念
 *
 * 本文件展示：
 * 1. 对象的创建和销毁
 * 2. 初始化vs赋值
 * 3. 合法值和不变性
 * 4. 继承关系（is-a和has-a）
 * 5. 类型转换
 * 6. 运算符重载
 */

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// 前向声明
class Engine;
class Wheel;

/**
 * @brief 演示资源管理的基础类
 * 展示：
 * - 构造函数和析构函数
 * - 拷贝控制
 * - 资源管理
 */
class ResourceManager {
  public:
  // 构造函数：初始化资源
  explicit ResourceManager(const std::string& name)
    : name_(name)
    , data_(new int[100]) {
    std::cout << "ResourceManager构造：" << name_ << std::endl;
  }

  // 析构函数：释放资源
  ~ResourceManager() {
    std::cout << "ResourceManager析构：" << name_ << std::endl;
    delete[] data_;
  }

  // 拷贝构造函数：深拷贝
  ResourceManager(const ResourceManager& other)
    : name_(other.name_ + "_copy")
    , data_(new int[100]) {
    std::copy(other.data_, other.data_ + 100, data_);
    std::cout << "ResourceManager拷贝构造：" << name_ << std::endl;
  }

  // 移动构造函数：转移资源所有权
  ResourceManager(ResourceManager&& other) noexcept
    : name_(std::move(other.name_))
    , data_(other.data_) {
    other.data_ = nullptr;   // 防止double-free
    std::cout << "ResourceManager移动构造：" << name_ << std::endl;
  }

  // 拷贝赋值运算符：深拷贝
  auto operator=(const ResourceManager& other) -> ResourceManager& {
    if (this != &other) {   // 自赋值检查
      delete[] data_;
      data_ = new int[100];
      std::copy(other.data_, other.data_ + 100, data_);
      name_ = other.name_ + "_copy";
    }
    std::cout << "ResourceManager拷贝赋值：" << name_ << std::endl;
    return *this;
  }

  // 移动赋值运算符：转移资源所有权
  auto operator=(ResourceManager&& other) noexcept -> ResourceManager& {
    if (this != &other) {
      delete[] data_;
      data_       = other.data_;
      other.data_ = nullptr;
      name_       = std::move(other.name_);
    }
    std::cout << "ResourceManager移动赋值：" << name_ << std::endl;
    return *this;
  }

  private:
  std::string name_;
  int*        data_;   // 模拟动态资源
};

/**
 * @brief 演示合法值和不变性的类
 * 展示：
 * - 构造函数参数验证
 * - setter函数验证
 * - 不变性维护
 */
class BankAccount {
  public:
  // 构造函数确保初始状态有效
  explicit BankAccount(double initialBalance) {
    if (initialBalance < 0) {
      throw std::invalid_argument("初始余额不能为负数");
    }
    balance_ = initialBalance;
  }

  // setter函数维护不变性
  void deposit(double amount) {
    if (amount <= 0) {
      throw std::invalid_argument("存款金额必须为正数");
    }
    balance_ += amount;
  }

  void withdraw(double amount) {
    if (amount <= 0) {
      throw std::invalid_argument("取款金额必须为正数");
    }
    if (balance_ - amount < 0) {
      throw std::runtime_error("余额不足");
    }
    balance_ -= amount;
  }

  // getter返回当前状态
  [[nodiscard]] auto getBalance() const -> double { return balance_; }

  private:
  double balance_;   // 不变性：余额永远不能为负
};

/**
 * @brief 演示继承关系的基类
 * 展示：
 * - 虚函数
 * - 纯虚函数
 * - protected成员
 */
class Vehicle {
  public:
  virtual ~Vehicle() = default;   // 虚析构函数

  // 纯虚函数：定义接口
  virtual void start() = 0;
  virtual void stop()  = 0;

  // 虚函数：可以被重写
  virtual void accelerate() { std::cout << "Vehicle加速" << std::endl; }

  // 非虚函数：不应被重写
  void park() { std::cout << "Vehicle停车" << std::endl; }

  protected:
  // 保护成员：派生类可访问
  bool engineRunning_ = false;
};

/**
 * @brief 演示"is-a"关系的派生类
 * Car "is-a" Vehicle
 */
class Car : public Vehicle {
  public:
  // 实现纯虚函数
  void start() override {
    std::cout << "Car启动" << std::endl;
    engineRunning_ = true;
  }

  void stop() override {
    std::cout << "Car停止" << std::endl;
    engineRunning_ = false;
  }

  // 重写虚函数
  void accelerate() override { std::cout << "Car加速" << std::endl; }
};

/**
 * @brief 演示"has-a"关系的类
 * Garage有多个Vehicle
 */
class Garage {
  public:
  // 添加车辆
  void addVehicle(std::unique_ptr<Vehicle> vehicle) { vehicles_.push_back(std::move(vehicle)); }

  // 启动所有车辆
  void startAllVehicles() {
    for (const auto& vehicle : vehicles_) {
      vehicle->start();   // 多态调用
    }
  }

  private:
  std::vector<std::unique_ptr<Vehicle>> vehicles_;   // has-a关系
};

/**
 * @brief 演示类型转换的类
 * 展示：
 * - 转换构造函数
 * - 转换运算符
 * - explicit关键字
 */
class Distance {
  public:
  // 转换构造函数：从int到Distance
  explicit Distance(int meters)
    : meters_(meters) {}

  // 转换运算符：从Distance到double
  explicit operator double() const {
    return meters_ / 1000.0;   // 转换为千米
  }

  // 常规成员函数
  [[nodiscard]] auto getMeters() const -> int { return meters_; }

  private:
  int meters_;
};

// 主函数：演示各种概念
int main() {
  try {
    // 1. 资源管理演示
    std::cout << "\n=== 资源管理演示 ===" << std::endl;
    ResourceManager rm1("Original");
    ResourceManager rm2 = std::move(rm1);   // 移动构造
    ResourceManager rm3("Another");
    rm3 = std::move(rm2);   // 移动赋值

    // 2. 合法值和不变性演示
    std::cout << "\n=== 合法值演示 ===" << std::endl;
    BankAccount account(1000.0);
    account.deposit(500.0);
    account.withdraw(200.0);
    std::cout << "当前余额: " << account.getBalance() << std::endl;

    try {
      account.withdraw(2000.0);   // 将抛出异常
    } catch (const std::runtime_error& e) {
      std::cout << "预期的异常：" << e.what() << std::endl;
    }

    // 3. 继承和多态演示
    std::cout << "\n=== 继承和多态演示 ===" << std::endl;
    Garage garage;
    garage.addVehicle(std::make_unique<Car>());
    garage.startAllVehicles();

    // 4. 类型转换演示
    std::cout << "\n=== 类型转换演示 ===" << std::endl;
    Distance dist(1500);
    double   km = static_cast<double>(dist);   // 显式转换
    std::cout << dist.getMeters() << "米 = " << km << "千米" << std::endl;

  } catch (const std::exception& e) {
    std::cerr << "错误：" << e.what() << std::endl;
    return 1;
  }

  return 0;
}