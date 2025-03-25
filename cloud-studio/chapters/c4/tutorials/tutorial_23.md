# 条款 23.抉择：类成员函数还是普通函数？

- 假设有一个表示浏览器的类，类中包括处理Cache、历史记录、Cookie的函数。

```cpp
class WebBrowser {
public:
...
void clearCache();
void clearHistory();
void removeCookies();
...
};
```

- 现在又需要做一个功能：删除所有浏览痕迹。这个功能可以由类成员函数实现或者普通函数实现。

```cpp
void clearBrowser(WebBrowser& wb)
{
wb.clearCache();
wb.clearHistory();
wb.removeCookies();
}
```

- 现在需要做出抉择：写成类成员函数还是普通函数呢？
- 根据常理，应当把数据和操作封装在一个类中，所以应该写在类里。 但是事实不是这样，实际上应该写在普通函数中。

从封装性角度考虑
- 首先引入一个衡量数据封装性的方法：有多少代码可以访问到该数据。封装性好意味着该数据耦合越小。**public/protected的数据完全没有封装性，而private的数据封装性取决于访问它的类成员函数以及友元函数的个数**。

所以可以看出：普通函数更有利于增加系统的封装性。

- 不过需要另外注意两点：一是友元函数和类成员函数没有什么区别，二是相对于当前类而言，其他类的成员函数也没有访问private变量的权限，所以也可以当作是普通函数。某些不允许声明普通函数的语言（比如Java）也可以声明为其他类的成员函数。

推荐写法
```cpp
namespace WebBrowserStuff {
class WebBrowser { ... };
void clearBrowser(WebBrowser& wb);
...
}
```

另一个原因：降低编译依存度
写在类外的函数有一个好处：可以划分为多个文件来编译。对于不同的用户，有可能只需要类的一部分功能，这时就只需要在不同头文件中分别声明执行某类功能的函数。

```cpp
// 头文件 "webbrowser.h" 定义WebBrowser类
// 以及一些核心的功能函数
namespace WebBrowserStuff {
class WebBrowser { ... };
... // "core" related functionality, e.g.
// non-member functions almost
// all clients need
}

// "webbrowserbookmarks.h"
namespace WebBrowserStuff {
... // 与书签相关的函数
} 
// header "webbrowsercookies.h"
namespace WebBrowserStuff {
... // cookie相关的函数
} 
```

- 类不能在多个文件中定义，但普通函数可以。并且可以把这些普通函数放在同一个namespace中。
- 其实stl就是这样做的。vector、list各自包含了不同的函数,需要使用哪个就使用哪个，他们其实共同用到了某些类。
- 并且，客户端可以自由地扩展namespace，以实现自己的功能，而类的扩展有种种限制。