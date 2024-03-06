# QT license说明



## 1. Reference：

- [qt license说明](https://www.qt.io/zh-cn/licensing)
- [查看哪些模块可以在LGPL和commercial等授权下使用](https://www.qt.io/zh-cn/product/features)
- [GNU许可证常见问题](https://www.gnu.org/licenses/gpl-faq.html)
- [qt法律纠纷](http://unitalen.com/html/report/16128208-1.htm)
- [为什么你的下一个库不应该使用 LGPL](https://www.gnu.org/licenses/why-not-lgpl.html)



注：commercial是商业授权



## 2. 开源使用职责
The majority of the Qt modules are available under the LGPL v3 and GPL v3 open source license. When developing under this license your obligations are to:

大多数Qt模块在LGPL v3和GPL v3开源许可证下可用。根据此许可证进行开发时，您的义务是：

- Provide a re-linking mechanism for Qt libraries

  以动态库连接的方式使用qt的库

- Provide a license copy & explicitly acknowledge Qt use

  提供一份license声明和明确声明使用了qt

- Make a Qt source code copy available for customers

  提供一份qt源码给用户

- Accept that Qt source code modifications are non-proprietary

  接受Qt源代码修改是非专有的，即修改源码之后需要开源

- Make “open” consumer devices

  制作开放的消费者设备（注： 这句话的含义是在倡导或要求生产能够让消费者自由访问、修改和定制其内部功能及软件的消费电子产品。这样的设备通常具备开源硬件（Open-Source Hardware）和开源软件（Open-Source Software）的特点，允许用户查看、修改源代码，并且能够基于这些开放的设计进行个性化创新或改进。）

- Accept Digital Rights Management terms, please see the GPL FAQ

  接受数字版权管理条款，请参考 [GPL FAQ](https://www.gnu.org/licenses/gpl-faq.html#DRMProhibited)

- Take special consideration when attempting to enforce software patents FAQ

  在试图强制执行软件专利时要特别考虑，请参考 [FAQ](https://www.gnu.org/licenses/gpl-faq.html#v3PatentRetaliation)



## 3. 如何在你的软件中使用 GNU 许可证

reference:

1. [如何在你的软件中使用 GNU 许可证](https://www.gnu.org/licenses/gpl-howto.html)
2. [GPL 的纯文本版](https://www.gnu.org/licenses/gpl.txt)
3. [LGPL 的纯文本版](https://www.gnu.org/licenses/lgpl.txt)

以下总结了在选择按照我们的许可证发布程序时，你需要做的事项：

- 从你的雇主或学校获取一份版权免除声明。
- 为每个文件提供适当的版权声明。确保 [用户可用的许可证版本被清晰地标识出来](https://www.gnu.org/licenses/identify-licenses-clearly.html)。
- 添加一个带有 GNU GPL 或 GNU AGPL 副本的 COPYING 文件。
- 如果使用了 GNU LGPL，那么再添加一个带有 GNU LGPL 副本的 COPYING.LESSER 文件。
- 每个文件都要有许可证声明。
- （可选项）程序开始时显示一个声明。
- （如果使用 AGPL）让程序带有提供源代码的选项。

这意味着每个程序都要添加两个单元：版权声明（比如 “Copyright 1999 Terry Jones”）和拷贝授权声明，说明本程序按照 GNU 通用公共许可证（或 LGPL、Affero GPL）的条款发布。



