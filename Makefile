# Makefile
CC := g++

SRC := Integer.hh Main.cc IntegerTools.hh

TARGET := test

$(TARGET) : $(SRC)
	$(CC) -o $(TARGET) $(SRC)

clean :
	rm $(TARGET)

PHONY : clean

