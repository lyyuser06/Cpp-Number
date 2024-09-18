#ifndef INTEGER_TOOLS_H_
#define INTEGER_TOOLS_H_ 1

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

class Debugger
{
  protected:
    bool _mode;
  
  public:
    Debugger() = default;
    Debugger(const bool &mode) : _mode(mode) {}
    ~Debugger() = default;

    bool mode() const { return _mode; }
    void debugModeOn() { _mode = true; }
    void debugModeOff() { _mode = false; }

    void printLine(int ln)
    {
      std::cout << "Print line at line " << ln << std::endl;
    }

    void printStringInfo(const char *p)
    {
      std::cout << "Data in char* p is " << p <<std::endl;      
    }

    void printInteger(bool sign)
    {
      std::cout << "Integer sign is " << sign << std::endl;
    }
};

Debugger debug;

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

/* 辅助工具：乘法表 */
constexpr int multable[10][10] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},  
  {0, 2, 4, 6, 8, 10, 12, 14, 16, 18},  
  {0, 3, 6, 9, 12, 15, 18, 21, 24, 27},  
  {0, 4, 8, 12, 16, 20, 24, 28, 32, 36},  
  {0, 5, 10, 15, 20, 25, 30, 35, 40, 45},  
  {0, 6, 12, 18, 24, 30, 36, 42, 48, 54},  
  {0, 7, 14, 21, 28, 35, 42, 49, 56, 63},  
  {0, 8, 16, 24, 32, 40, 48, 56, 64, 72},  
  {0, 9, 18, 27, 36, 45, 54, 63, 72, 81}  
};

/* 辅助函数：int 2 c_str, max_int to min_int - 1 */

char* i_2_a(int i)
{
  if(i < 0) i = -i;
  int max_len = 10;
  char array[max_len + 1];
  std::memset(array, '0', sizeof(array));
  int idx = max_len - 1;
  while(i > 0)
  {
    int digit = i % 10;
    char digit_num = digit + ASCII_NUMBER_OFFSET;
    array[idx] = digit_num;
    i /= 10;
    --idx;
  }
  array[max_len] = '\0';

  int len = max_len - 1 - idx;
  char *ret = new char[len];
  std::strcpy(ret, array + idx);
  return ret;
}

/* 高精度乘法 */
char* multiplyNumsOperand(const char* const &a, const char* const &b)
{
  int len_a = std::strlen(a), len_b = std::strlen(b);
  int offset  = 0, carry = 0;
  bool is_a_longer = (len_a > len_b) ? true : false;
  int len_shorter = (is_a_longer) ? len_b : len_a;
  int len_longer = std::max(len_a, len_b);

  char inter[len_shorter][len_longer + len_shorter + 2];
  std::memset(inter, '0', sizeof(inter));

  for(int i = 0; i < len_shorter; i++)
    inter[i][len_longer + len_shorter + 1] = '\0';
  
  for(int i = 0; i < len_shorter; i++)
  {
    for(int j = 0; j < len_longer; j++)
    {
      if(is_a_longer)
      {
        int digit_b = b[len_shorter - 1 - i] - ASCII_NUMBER_OFFSET;
        int digit_a = a[len_longer - 1 - j] - ASCII_NUMBER_OFFSET;

        /*
        if(debug.mode())
        {
          debug.printLine(digit_a);
          debug.printLine(digit_b);
        }
        */

        int digit_product = multable[digit_b][digit_a] + carry;

        inter[i][len_shorter - offset + len_longer - j] = (digit_product % 10) + ASCII_NUMBER_OFFSET;
        carry = digit_product / 10;
      }
      else
      {
        int digit_b = b[len_longer - 1 - j] - ASCII_NUMBER_OFFSET;
        int digit_a = a[len_shorter - 1 - i] - ASCII_NUMBER_OFFSET;

        /*
        if(debug.mode())
        {
          debug.printLine(digit_a);
          debug.printLine(digit_b);
        }
        */

        int digit_product = multable[digit_b][digit_a] + carry;

        inter[i][len_shorter + len_longer - offset - j] = (digit_product % 10) + ASCII_NUMBER_OFFSET;

        carry = digit_product / 10;

        /*
        if(debug.mode())
        {
          std::cout << "inter[i] is " << inter[i][len_longer - 1 - offset - j] << 
            " and carry is " << carry << std::endl;
        }
        */
      }

      /*
      if(debug.mode())
      {
        std::cout << "inter[i][len_longer - 1 - offset - j] is " << inter[i][len_longer - 1 - offset - j] << 
          " and carry is " << carry << std::endl;
      }
      */
    }

    if(carry > 0)
      inter[i][len_shorter - offset] = carry + ASCII_NUMBER_OFFSET;

    /*
    if(debug.mode())
      debug.printStringInfo(inter[i]);
    */

    ++offset;
    carry = 0;
  }

  for(int i = 0; i < len_shorter; i++)
    inter[i][len_longer + len_shorter + 1] = '\0';
  
  /*
  if(debug.mode())
    for(int i = 0; i < len_shorter; i++)
      debug.printStringInfo(inter[i]);
  */
  
  char *res = new char[len_longer + len_shorter + 2];
  std::memset(res, '0', sizeof(res));
  res[len_longer + len_shorter + 1] = '\0';
  for(int i = 0; i < len_shorter; i++)
  {
    char *num = addNumsOperand(res, inter[i]);
    /*
    if(debug.mode())
      debug.printStringInfo(num);
    */
    std::strcpy(res, num);
    delete num;
  }

  char *shrink = shrinkNumAtStart(res);
  delete res;

  return shrink;
}

#endif