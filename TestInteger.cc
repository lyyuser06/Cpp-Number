#include "Integer.hh"
#include <pybind11/pybind11.h>

void testAdd(const char* a, const char* b)
{
  Integer a1(true, a);
  Integer b1(true, b);
  Integer c = a1 + b1;
}

void testMinus(const char* a, const char* b)
{
  Integer a1(true, a);
  Integer b1(true, b);
  Integer c = a1 - b1;
}

PYBIND11_MODULE(test_integer, m) 
{
  m.def("test_add", &testAdd, "A function that test adding two numbers");
  m.def("test_minus", &testMinus, "A function that test differring two numbers");
}
