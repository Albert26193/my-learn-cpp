#include <iostream>
#include <string>
#include <vector>

// 命名空间用于组织相关的类和函数
namespace WebBrowserStuff {

/**
 * WebBrowser类：模拟一个网页浏览器的基本功能
 * 包含了核心功能如缓存、历史记录和Cookie管理
 */
class WebBrowser {
  public:
  // 构造函数：初始化浏览器的基本状态
  WebBrowser()
    : cacheSize_(0)
    , historyEntryCount_(0)
    , cookieCount_(0) {}

  // 核心功能：清除缓存
  void clearCache() {
    std::cout << "Clearing " << cacheSize_ << "MB of cache..." << std::endl;
    cacheSize_ = 0;
  }

  // 核心功能：清除历史记录
  void clearHistory() {
    std::cout << "Clearing " << historyEntryCount_ << " history entries..." << std::endl;
    historyEntryCount_ = 0;
  }

  // 核心功能：移除Cookie
  void removeCookies() {
    std::cout << "Removing " << cookieCount_ << " cookies..." << std::endl;
    cookieCount_ = 0;
  }

  // 模拟浏览网页，增加各种数据
  void browseWebPage(const std::string& url) {
    std::cout << "Browsing: " << url << std::endl;
    cacheSize_ += 10;          // 假设每个页面产生10MB缓存
    historyEntryCount_ += 1;   // 增加一条历史记录
    cookieCount_ += 2;         // 假设每个页面产生2个cookie
  }

  // Getter函数用于展示当前状态
  void showStatus() const {
    std::cout << "Browser Status:\n"
              << "Cache Size: " << cacheSize_ << "MB\n"
              << "History Entries: " << historyEntryCount_ << "\n"
              << "Cookies: " << cookieCount_ << "\n";
  }

  private:
  int cacheSize_;           // 缓存大小（MB）
  int historyEntryCount_;   // 历史记录数量
  int cookieCount_;         // Cookie数量
};

// 非成员函数：一次性清除所有浏览数据
// 这是一个普通函数而不是类成员函数，原因如下：
// 1. 提高封装性：不需要访问类的私有成员
// 2. 降低编译依存度：可以在单独的文件中编译
// 3. 提供更好的包组织：可以根据功能分组在不同的头文件中

/**
  封装性：public/protected的数据完全没有封装性，
  而private的数据封装性取决于访问它的类成员函数以及友元函数的个数

  也就是说，越少的访问点，越有利于封装性。
  如果把 clearBrowser 放在 WebBrowser 类中，那么它需要访问 WebBrowser 的私有成员，
  这样就增加了访问点，不利于封装性。反之，通过调用方法去访问私有成员，
  这样就减少了访问点，有利于封装性。
 */

void clearBrowser(WebBrowser& wb) {
  wb.clearCache();
  wb.clearHistory();
  wb.removeCookies();
}

// 可以根据需要添加更多的非成员函数，比如：
void clearBrowserAndReport(WebBrowser& wb) {
  std::cout << "=== Before Clearing ===" << std::endl;
  wb.showStatus();

  clearBrowser(wb);

  std::cout << "=== After Clearing ===" << std::endl;
  wb.showStatus();
}

}   // namespace WebBrowserStuff

// 演示代码
auto main() -> int {
  using namespace WebBrowserStuff;

  // 创建浏览器实例
  WebBrowser browser;

  // 模拟浏览几个网页
  browser.browseWebPage("https://www.example.com");
  browser.browseWebPage("https://www.google.com");
  browser.browseWebPage("https://www.github.com");

  // 使用非成员函数来清理浏览器
  clearBrowserAndReport(browser);

  return 0;
}