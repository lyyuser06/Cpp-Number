#ifndef INTEGER_H_
#define INTEGER_H_ 1

#include "IntegerTools.hh"

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

    void zeroSign() 
    {
      if(std::strcmp(_data, "0") == 0)
        _sign = true;
    }

    Integer& operator=(const Integer &integer)
    {
      if(this != &integer)
      {
        delete _data;
        copyInteger(integer._sign, integer._data);
      }
      zeroSign();
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
      zeroSign();
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
      sum.zeroSign();
      return sum;
    }

    Integer operator-(const Integer &other_int) const
    {
      Integer that(other_int);
      that.setSign(!that.getSign());
      Integer diff =  ((*this) + that);
      diff.zeroSign();
      return diff;
    }

    Integer operator*(const Integer &other_int) const
    {
      Integer pro = multiplyNums((*this), other_int);
      if((_sign && !other_int._sign) || (!_sign && other_int._sign))
        pro.setSign(false);
      pro.zeroSign();
      return pro;
    }

    friend std::ostream& operator<<(std::ostream &os, const Integer &integer);
    friend Integer addNums(const Integer &this_int, const Integer &other_int);
    friend Integer minusNums(const Integer &this_int, const Integer &other_int);
    friend Integer multiplyNums(const Integer &a, const Integer &b);
};

extern Debugger debug;

std::ostream& operator<<(std::ostream &os, const Integer &integer)
{
  if(!integer._sign) 
    os << "-";
  os << integer._data << std::endl;
  return os;
}

Integer addNums(const Integer &this_int, const Integer &other_int)
{
  char *sum = addNumsOperand(this_int._data, other_int._data);
  Integer ret(true, sum);
  delete sum;
  return ret;
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

Integer multiplyNums(const Integer &a, const Integer &b)
{
  char *product = multiplyNumsOperand(a._data, b._data);
  Integer ret(true, product);
  delete product;
  return ret;
}

#endif