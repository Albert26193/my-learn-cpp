#include <iostream>
#include <memory>

using namespace std;

// bad case
class BadDate {
  public:
  BadDate(int month, int day, int year)
    : month(month)
    , day(day)
    , year(year){};
  int month;
  int day;
  int year;
};

// good case
struct Month {
  explicit Month(int m)
    : val(m){};
  int val;
};
struct Day {
  explicit Day(int d)
    : val(d){};
  int val;
};
struct Year {
  explicit Year(int y)
    : val(y){};
  int val;
};

// 参数使用包装类型
class GoodDate {
  public:
  GoodDate(const Month& month, const Day& day, const Year& year)
    : month(month.val)
    , day(day.val)
    , year(year.val){};
  int month;
  int day;
  int year;
};

auto main() -> int {
  const int year  = 2023;
  const int month = 12;
  const int day   = 7;

  // BadDate
  // BadDate badDate(year, month, day);
  // cout << badDate.year << "/" << badDate.month << "/" << badDate.day << endl;

  // GoodDate
  // GoodDate goodDate(month, year, day); // can't compile
  // GoodDate goodDate(Year(year), Month(month), Day(day)); // can't compile
  GoodDate goodDate((Month(month)), Day(day), Year(year));   // ok
  cout << goodDate.year << "/" << goodDate.month << "/" << goodDate.day << endl;

  // discuss：Date 是否还有更合理的写法？
  // 1. 使用命名构造函数（Named Constructor Idiom）
  // class Date {
  // public:
  //   static Date createDate(int year, int month, int day) {
  //     // 在这里可以进行参数验证
  //     return Date(year, month, day);
  //   }
  // private:
  //   Date(int y, int m, int d) : year(y), month(m), day(d) {}
  // };
  //
  // 2. 使用Builder模式
  // Date d = DateBuilder().setYear(2023).setMonth(12).setDay(7).build();
  //
  // 3. 使用工厂函数和验证
  // std::optional<Date> makeDate(int year, int month, int day) {
  //   if (month < 1 || month > 12 || day < 1 || day > 31) return std::nullopt;
  //   return Date(year, month, day);
  // }
  //
  // 4. 使用枚举类型代替简单整数
  // enum class Month { Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };
  // Date d(2023, Month::Dec, 7);

  return 0;
}