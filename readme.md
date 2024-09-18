# C++ 数字类型

平台：WSL2 Ubuntu 22.04
仿照Java语言的Integer类等Number类，进行C++数字类的实现。

## 整数部分（Integer）
提供了大整数类（Integer），支持 字符串/int类型 初始化，支持和int（4 Byte）类型的转换，支持输出到输出流对象。

接口：
> int getVal(void)

> ostream& operator<<(ostream &, const Integer &)

> 赋值符号和移动赋值

> \+ \- \* 运算符

文件结构：
> Integer.hh 目前已实现的整数类

> IntegerTools.hh 需要使用的各种辅助工具函数

> Main.cc 调试用主程序

> readme.md 文档

> Makefile 工程构建文件

> TestInteger.cc C++语言编写的检测脚本，需要编译成 CPython 库

> TestInteger.py 测量运行时间的Python脚本

未实现的部分：整数整除法。