#include "Integer.hh"

int main()
{
  Integer i1(-12);
  Integer i2(0);
  Integer i3(12);
  Integer i4(2147483647);
  Integer i5(-2147483648);

  Integer i1c("-123");
  Integer i2c("0");
  Integer i3c("123");
  Integer i4c("21474836470");
  Integer i5c("-21474836480");

  Integer i1cpy(i1);
  Integer i2cpy(i2);
  Integer i5ccpy(i5c);

  std::cout << i1 << i2 << i3 << i4 << i5 << i1c << i2c << i3c << i4c << i5c
    << i1cpy << i2cpy << i5ccpy;

  int v1 = i4.getVal();
  int v2 = i5.getVal();

  std::cout << v1 << " " << v2 << std::endl;

  Integer v1c = i1c;
  Integer v5c = i5c;

  std::cout << v1c << v5c << std::endl;

  Integer k("2147483648");
  //std::cout << k.getVal() << std::endl;
  return 0;
}