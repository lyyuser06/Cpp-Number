# Makefile

# Python3 和 pybind11 的编译器标志
PYTHON_INCLUDES := $(shell python3 -m pybind11 --includes)
PYTHON_EXTENSION_SUFFIX := $(shell python3-config --extension-suffix)

# 编译器
CXX := c++
CXXFLAGS := -O3 -Wall -shared -std=c++11 -fPIC

# 目标模块名
TARGET := test_integer$(PYTHON_EXTENSION_SUFFIX)

# 源文件
SRC := TestInteger.cc Integer.hh

# 目标规则
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) $(SRC) -o $(TARGET)

# 清理规则
.PHONY: clean
clean:
	rm -f $(TARGET)
