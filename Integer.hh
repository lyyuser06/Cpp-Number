#ifndef INTEGER_H_
#define INTEGER_H_ 1

#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <cstring>
#include <stdexcept>

#define ASCII_NUMBER_OFFSET 48

class Integer
{
  protected:
    bool _sign;
    char *_data;
    int _length;
    
    void copyInteger(const bool &sign, char* const &data)
    {
      _sign = sign;
      _length = strlen(data);
      _data = new char[_length + 1];
      strcpy(_data, data);
    }

    bool overIntJudge() const
    {
      if(_length < 10) 
        return false;
      else if(_length == 10)
      {
        const char *max_int_plus = "2147483647";
        const char *max_int_minus = "2147483648";

        if(_sign && (strcmp(_data, max_int_plus) <= 0)) 
          return false;
        else if(!_sign && (strcmp(_data, max_int_minus) <= 0))
          return false; 
      }   
      
      return true;
    }

  public:
    Integer() : _length(0), _data(nullptr), _sign(false) {}
    Integer(const bool &sign, char* const &data) { copyInteger(sign, data); }
    Integer(const Integer &integer) { copyInteger(integer._sign, integer._data); }
    Integer(Integer &&integer) : _sign(integer._sign), _length(integer._length), 
      _data(integer._data) { integer._data = nullptr; }
    Integer(const char* const &integer)
    {
      if(integer[0] == '-')
      {
        _sign = false;
        _length = strlen(integer) - 1;
        _data = new char[_length + 1];
        strcpy(_data, integer + 1);
      }
      else
      {
        _sign = true;
        _length = strlen(integer);
        _data = new char[_length + 1];
        strcpy(_data, integer);
      }
    }
    Integer(int value)
    {
      int off = 1;
      if(value < 0 && value != -2147483648)
      {
        value = -value;
        _sign = false;
      }
      else if(value == -2147483648)
      {
        off = -1;
        _sign = false;
      }
      else _sign = true;

      std::vector<char> digit_stack;
      do {
        int digit = (value % 10) * off;
        char ch = digit + ASCII_NUMBER_OFFSET;
        digit_stack.push_back(ch);
        value /= 10;
      } while(value * off > 0);
      
      _length = digit_stack.size();
      _data = new char[_length + 1];

      auto it = digit_stack.rbegin();
      int idx = 0;
      while(it != digit_stack.rend())
      {
        _data[idx] = (*it);
        ++it; ++idx;
      }
      _data[idx] = '\0';
    }

    ~Integer() { delete _data; }

    Integer& operator=(const Integer &integer)
    {
      copyInteger(integer._sign, integer._data);
      return *this;
    }
    Integer& operator=(Integer &&integer)
    {
      if(this != &integer)
      {
        _sign = integer._sign;
        _length = integer._length;
        delete _data;
        _data = integer._data;
        integer._data = nullptr;
      }
      return *this;
    }

    /* Members */
    int getVal() const
    {
      if(overIntJudge())
        throw std::runtime_error("Error:Over Int Range!\n");
      
      
      std::stack<char> digit_stack;

      for(int i = 0; i < _length; i++)
        digit_stack.push(_data[i]);
      
      int value = 0; int power = 1;
      while(!digit_stack.empty())
      {
        char digit = digit_stack.top();
        value = value + power * (digit - ASCII_NUMBER_OFFSET);
        power = power * 10;
        digit_stack.pop();
      }

      if(!_sign) value = -value;
      return value;
    }

    friend std::ostream& operator<<(std::ostream &os, const Integer &integer);
};

std::ostream& operator<<(std::ostream &os, const Integer &integer)
{
  if(!integer._sign) 
    os << "-";
  os << integer._data << std::endl;
  return os;
}

#endif