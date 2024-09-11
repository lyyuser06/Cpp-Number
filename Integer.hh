#ifndef INTEGER_H_
#define INTEGER_H_ 1

#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <memory>
#include <stdexcept>

#define ASCII_NUMBER_OFFSET 48

class Integer
{
  protected:
    bool _sign;
    char *_data;
    int _length;
    
    void copyInteger(const bool &sign, const char* const &data)
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
    Integer() : _sign(false), _data(nullptr), _length(0) {}
    Integer(const bool &sign, const char* const &data) { copyInteger(sign, data); }
    Integer(const Integer &integer) { copyInteger(integer._sign, integer._data); }
    Integer(Integer &&integer) : _sign(integer._sign), _data(integer._data), _length(integer._length)
       { integer._data = nullptr; }
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

    bool getSign() const { return _sign; }

    bool setSign(const bool &s) 
    {
      _sign = s; 
      return _sign; 
    }

    const char* getNum() const { return _data; }

    /* 高精度四则运算：加、减、乘、整除 */

    Integer operator+(const Integer &other_int) const
    {
      if(_sign && other_int._sign)
        return addNums((*this), other_int);
      else if(!_sign && !other_int._sign)
      {
        Integer this_num(*this), that_num(other_int);
        this_num.setSign(true);
        that_num.setSign(true);
        Integer sum = addNums(this_num, that_num);
        sum.setSign(false);
        return sum;
      }
      
      Integer sum = minusNums((*this), other_int);
      return sum;
    }

    Integer operator-(const Integer &other_int) const
    {
      Integer that(other_int);
      that.setSign(!that.getSign());
      return ((*this) + that);
    }

    friend std::ostream& operator<<(std::ostream &os, const Integer &integer);
    friend Integer addNums(const Integer &this_int, const Integer &other_int);
    friend Integer minusNums(const Integer &this_int, const Integer &other_int);
};

std::ostream& operator<<(std::ostream &os, const Integer &integer)
{
  if(!integer._sign) 
    os << "-";
  os << integer._data << std::endl;
  return os;
}

/* 
 *  辅助函数：正数相加。
 */

char* addNumsOperand(const char* const &a, const char* const &b)
{
    int len_a = std::strlen(a), len_b = std::strlen(b);
    int carry = 0;
    std::vector<char> sum_vec; 
    while(len_a > 0 && len_b > 0)
    {
        int sum_digit = (a[len_a - 1] - ASCII_NUMBER_OFFSET) + 
            (b[len_b - 1] - ASCII_NUMBER_OFFSET) + carry;
        if(sum_digit >= 10)
        {
            carry = 1;
            sum_digit -= 10;
        }
        else carry = 0;

        char digit = sum_digit + ASCII_NUMBER_OFFSET;
        sum_vec.push_back(digit);
        --len_a; --len_b;
    }

    bool is_a_longer = (len_b == 0);
    int difference = (is_a_longer) ? len_a : len_b;
    while(difference > 0)
    {
        if(is_a_longer)
        {
            int sum_digit = (a[difference - 1] - ASCII_NUMBER_OFFSET) + carry;
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
            int sum_digit = (b[difference - 1] - ASCII_NUMBER_OFFSET) + carry;
            if(sum_digit >= 10)
            {
                carry = 1;
                sum_digit -= 10;
            }
            else carry = 0;

            char digit = sum_digit + ASCII_NUMBER_OFFSET;
            sum_vec.push_back(digit);
        }
        --difference;
    }

    if(carry == 1) 
        sum_vec.push_back('1');
            
    int len_sum = sum_vec.size();
    char *sum_array = new char[len_sum + 1]; int idx = 0;
    for(auto it = sum_vec.rbegin(); it != sum_vec.rend(); ++it)
    {
        sum_array[idx] = (*it);
        ++idx;
    }
    sum_array[len_sum] = '\0';

    return sum_array;
}

Integer addNums(const Integer &this_int, const Integer &other_int)
{
  char *sum = addNumsOperand(this_int._data, other_int._data);
  Integer ret(true, sum);
  delete sum;
  return ret;
}

/* 辅助函数：数字字符串开头扩展位数，并补齐前导0。缩减位数，并删除前导0。*/

char* expandNumAtStart(const char* const &num, int difference)
{
  int new_len = std::strlen(num) + difference;
  char *expand = new char[new_len];
  std::strcpy(expand + difference, num);
  std::memset(expand, '0', difference * sizeof(char));
  return expand;
}

char* shrinkNumAtStart(const char* const &num)
{
  int idx = 0;
  int old_len = std::strlen(num);
  while(idx < old_len && num[idx] == '0')
    idx++;

  if(idx == old_len)
  {
    char *shrink = new char[2];
    shrink[0] = '0'; shrink[1] = '\0';
    return shrink;
  }

  char *shrink = new char[old_len - idx];
  std::strcpy(shrink, num + idx);
  return shrink;
}

/* 
 *  辅助函数：正数相减，大数减小数。
 *  小数的位数不超过大数，且如果小于大数的话，位数进行前导补0操作。
 *  本函数的两个参数数组长度相同，因此需要事先进行补齐0的操作，如果
 *  长度相同，那么不用补齐。
 */

char* minusNumsOperand(const char* const &big, const char* const &small)
{
  int len = std::strlen(big);
  int borrow = 0; char *diff = new char[len + 1];
  diff[len] = '\0';

  while(len > 0)
  {
    int digit_num = (big[len - 1] - borrow) - small[len - 1];
    if(digit_num < 0)
    {
      digit_num += 10;
      borrow = 1;
    }
    else borrow = 0;
    diff[len - 1] = digit_num + ASCII_NUMBER_OFFSET;
    --len;
  }
  return diff;
}

Integer minusNums(const Integer &this_int, const Integer &other_int)
{
  if(std::strcmp(this_int._data, other_int._data) == 0)
    return Integer(0);

  int len_this = std::strlen(this_int._data), len_other = std::strlen(other_int._data);
  int difference = len_this - len_other;

  /* 长度比较，1：this比other长；0：一样长；-1：this比other短 */
  int len_cmp = 0; bool this_abs_bigger = false;
  if(difference > 0) 
  {
    len_cmp = 1;
    this_abs_bigger = true;
  }
  else if(difference == 0)
  {
    len_cmp = 0;
    this_abs_bigger = (std::strcmp(this_int._data, other_int._data) > 0) ? true : false;
  }
  else 
  {
    difference = -difference;
    len_cmp = -1;
    this_abs_bigger = false;
  }

  /* 扩展：当this长于other时扩展other，其余情况扩展this。 */
  char *expand;
  if(len_cmp > 0) expand = expandNumAtStart(other_int._data, difference);
  else expand = expandNumAtStart(this_int._data, difference);

  char *diff;
  if(len_cmp == 0 && this_abs_bigger) 
    diff = minusNumsOperand(expand, other_int._data);
  else if(len_cmp == 0 && !this_abs_bigger) 
    diff = minusNumsOperand(other_int._data, expand);
  else if(len_cmp > 0)
    diff = minusNumsOperand(this_int._data, expand);
  else if(len_cmp < 0)
    diff = minusNumsOperand(other_int._data, expand);

  if(this_abs_bigger && this_int._sign && !other_int._sign)
  {
    char *shrink = shrinkNumAtStart(diff);
    Integer ret(true, shrink);
    delete expand; delete diff; 
    delete shrink;
    return ret;
  }
  else if(this_abs_bigger && !this_int._sign && other_int._sign)
  {
    char *shrink = shrinkNumAtStart(diff);
    Integer ret(false, shrink);
    delete expand; delete diff; 
    delete shrink;
    return ret;
  }
  else if(!this_abs_bigger && this_int._sign && !other_int._sign)
  {
    char *shrink = shrinkNumAtStart(diff);
    Integer ret(false, shrink);
    delete expand; delete diff; 
    delete shrink;
    return ret;
  }
  
  /* 结果缩位 */
  char *shrink = shrinkNumAtStart(diff);
  Integer ret(true, shrink);
  delete expand; delete diff; 
  delete shrink;
  return ret;
}

/* 辅助函数: 在数字字符串末尾按位数扩展，添加后置0；缩减，删除0。 */

char* expandNumAtEnd(const char* const &num, int difference)
{
  int old_len = std::strlen(num);
  int new_len = old_len + difference;
  char *expand = new char[new_len + 1];
  std::strcpy(expand, num);
  std::memset(expand + old_len, '0', difference);
  expand[new_len] = '\0';
  return expand;
}

char* shrinkNumAtEnd(const char* const &num)
{
  int idx = std::strlen(num) - 1;
  while(idx > 0)
  {
    if(num[idx] != '0') break;
    --idx;
  }

  char *shrink = new char[idx + 2];
  for(int i = 0; i <= idx; i++)
    shrink[i] = num[i];
  shrink[idx + 1] = '\0';
  return shrink;
}

char* multiplyNums(const char* const &a, const char* const &b)
{

}


#endif