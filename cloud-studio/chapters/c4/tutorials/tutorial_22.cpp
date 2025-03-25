#include <iostream>
#include <string>
#include <utility>

// 反面教材：公开成员变量
class BadPerson {
  public:
  std::string name;     // 不好：直接暴露成员变量
  int         age;      // 不好：无法控制有效范围
  double      salary;   // 不好：无法实施访问控制
};

// 反面教材：使用protected成员变量
class BadEmployee {
  protected:
  std::string name;     // 不好：派生类可以直接访问
  int         age;      // 不好：破坏封装性
  double      salary;   // 不好：难以维护约束条件
};

class BadManager : public BadEmployee {
  public:
  void giveSalaryRaise() {
    salary *= 1.1;   // 可以直接访问基类的protected成员
                     // 问题：无法确保salary的有效性
    // 问题：如果未来想改变salary的存储方式，所有派生类都需要修改
  }
};

// 正面教材：使用private成员变量和公共接口
class Person {
  public:
  Person(std::string name, int age)
    : name_(std::move(name))
    , age_(age) {
    setAge(age);   // 通过setter确保age的有效性
  }

  // 只读访问
  [[nodiscard]] auto getName() const -> const std::string& { return name_; }

  // 读写访问（带验证）
  [[nodiscard]] auto getAge() const -> int { return age_; }
  auto               setAge(int newAge) -> void {
                  if (newAge < 0 || newAge > 150) {
                    throw std::invalid_argument("Age must be between 0 and 150");
    }
                  age_ = newAge;
  }

  private:
  std::string name_;
  int         age_;
};

// 正面教材：展示实现的灵活性
class Employee {
  public:
  Employee(std::string name, double baseSalary)
    : name_(std::move(name))
    , baseSalary_(baseSalary) {}

  // 只读访问
  [[nodiscard]] auto getName() const -> const std::string& { return name_; }

  // 计算实际工资（可以随时改变实现而不影响客户端代码）
  [[nodiscard]] auto getCurrentSalary() const -> double { return baseSalary_ * (1.0 + bonusRate_); }

  // 设置奖金率
  auto setBonusRate(double rate) -> void {
    if (rate < 0.0 || rate > 1.0) {
      throw std::invalid_argument("Bonus rate must be between 0 and 1");
    }
    bonusRate_ = rate;
  }

  private:
  std::string name_;
  double      baseSalary_;
  double      bonusRate_ = 0.0;   // 默认无奖金
};

// 展示如何通过private成员实现更复杂的约束条件
class BankAccount {
  public:
  BankAccount(std::string owner, double initialBalance = 0.0)
    : owner_(std::move(owner)) {
    setBalance(initialBalance);
  }

  // 只读访问
  [[nodiscard]] auto getOwner() const -> const std::string& { return owner_; }
  [[nodiscard]] auto getBalance() const -> double { return balance_; }

  // 存款操作
  auto deposit(double amount) -> void {
    if (amount <= 0) {
      throw std::invalid_argument("Deposit amount must be positive");
    }
    setBalance(balance_ + amount);
  }

  // 取款操作
  auto withdraw(double amount) -> void {
    if (amount <= 0) {
      throw std::invalid_argument("Withdrawal amount must be positive");
    }
    if (amount > balance_) {
      throw std::runtime_error("Insufficient funds");
    }
    setBalance(balance_ - amount);
  }

  private:
  std::string owner_;
  double      balance_ = 0.0;

  // 私有辅助函数，确保余额更新的一致性
  auto setBalance(double newBalance) -> void {
    if (newBalance < 0) {
      throw std::invalid_argument("Balance cannot be negative");
    }
    balance_ = newBalance;
    // 这里可以添加其他逻辑，如记录交易、通知观察者等
  }
};

auto main() -> int {
  try {
    std::cout << "演示正确的封装：\n";

    // 使用Person类
    Person person("Alice", 30);
    std::cout << person.getName() << " is " << person.getAge() << " years old\n";

    try {
      person.setAge(200);   // 会抛出异常
    } catch (const std::exception& e) {
      std::cout << "验证年龄：" << e.what() << "\n";
    }

    // 使用Employee类
    Employee emp("Bob", 5000);
    emp.setBonusRate(0.1);   // 10%的奖金
    std::cout << emp.getName() << "'s current salary: " << emp.getCurrentSalary() << "\n";

    // 使用BankAccount类
    BankAccount account("Charlie", 1000);
    account.deposit(500);
    std::cout << "After deposit: " << account.getBalance() << "\n";

    account.withdraw(300);
    std::cout << "After withdrawal: " << account.getBalance() << "\n";

    try {
      account.withdraw(2000);   // 会抛出异常
    } catch (const std::exception& e) {
      std::cout << "验证取款：" << e.what() << "\n";
    }

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
