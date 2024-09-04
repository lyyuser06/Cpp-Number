# C++ 数字类型

仿照Java语言的Integer类等Number类，进行C++数字类的实现。

## 整数部分（Integer）
提供了大整数类（Integer），支持 字符串/int类型 初始化，支持和int（4 Byte）类型的转换，支持输出到输出流对象。

接口：
> int getVal(void)
> ostream& opeartor<<(ostream &, const Integer &)
> 赋值符号和移动赋值

文件结构：
> Integer.hh 目前已实现的整数类雏形
> Main.cc 调试用主程序
> readme.md 文档

展望：高精度运算；有理数、实数部分。