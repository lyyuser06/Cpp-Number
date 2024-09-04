#ifndef INTEGER_H_
#define INTEGER_H_ 1

#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <algorithm>
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
      
      int off = 1;
      if(!_sign && (strcmp(_data, "2147483648") == 0))
        off = -1;
      int value = 0; int power = 1;
      while(!digit_stack.empty())
      {
        char digit = digit_stack.top();
        value = value + power * (digit - ASCII_NUMBER_OFFSET) * off;
        power = power * 10;
        digit_stack.pop();
      }

      if(!_sign && (off == 1)) 
        value = -value;
      return value;
    }

    /* 高精度四则运算：加、减、乘、整除 */

    Integer operator+(const Integer &other_int)
    {
      if(_sign && other_int._sign)
        return plusplusNumsAdd((*this), other_int);
      
    }
    friend std::ostream& operator<<(std::ostream &os, const Integer &integer);
    friend Integer plusplusNumsAdd(const Integer &this_int, const Integer &other_int);
    friend Integer plusMinusNumsAdd(const Integer &this_int, const Integer &other_int);
};

std::ostream& operator<<(std::ostream &os, const Integer &integer)
{
  if(!integer._sign) 
    os << "-";
  os << integer._data << std::endl;
  return os;
}

Integer plusplusNumsAdd(const Integer &this_int, const Integer &other_int)
{
    int len = this_int._length, len_other = other_int._length;
    int carry = 0;
    std::vector<char> sum_vec; 
    while(len > 0 && len_other > 0)
    {
        int sum_digit = (this_int._data[len - 1] - ASCII_NUMBER_OFFSET) + 
            (other_int._data[len_other - 1] - ASCII_NUMBER_OFFSET) + carry;
        if(sum_digit >= 10)
        {
            carry = 1;
            sum_digit -= 10;
        }
        else carry = 0;

        char digit = sum_digit + ASCII_NUMBER_OFFSET;
        sum_vec.push_back(digit);
        --len; --len_other;
    }

    int len_longer = std::max(len, len_other);
    bool is_this_longer = (len_longer == len) ? true : false;
    while(len_longer > 0)
    {
        if(is_this_longer)
        {
            int sum_digit = (this_int._data[len_longer - 1] - ASCII_NUMBER_OFFSET) + carry;
            if(sum_digit >= 10)
            {
                carry = 1;
                sum_digit -= 10;
            }
            char digit = sum_digit + ASCII_NUMBER_OFFSET;
            sum_vec.push_back(digit);
        }
        else
        {
            int sum_digit = (other_int._data[len_longer - 1] - ASCII_NUMBER_OFFSET) + carry;
            if(sum_digit >= 10)
            {
                carry = 1;
                sum_digit -= 10;
            }
            else carry = 0;

            char digit = sum_digit + ASCII_NUMBER_OFFSET;
            sum_vec.push_back(digit);
        }
            --len_longer;
    }

    if(carry == 1) 
        sum_vec.push_back('1');
            
    int len_sum = sum_vec.size();
    char sum_array[len_sum + 1]; int idx = 0;
    for(auto it = sum_vec.rbegin(); it != sum_vec.rend(); ++it)
    {
        sum_array[idx] = (*it);
        ++idx;
    }
    sum_array[len_sum] = '\0';

    Integer sum(true, sum_array);
    return sum;
}

Integer plusMinusNumsAdd(const Integer &this_int, const Integer &other_int)
{
    int len_this = this_int._length, len_other = other_int._length;
    bool is_len_equal;
}

#endif