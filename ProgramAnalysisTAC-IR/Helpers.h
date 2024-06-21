#include "Instructions.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

bool IsArithmaticOrLogicalOpreation(string code) {
  return (code == "+" or code == "-" or code == "*" or code == "/" or
          code == "%" or code == "^" or code == "|" or code == "&" or
          code == "&&" or code == "||" or code == ">>" or code == "<<" or
          code == ">=" or code == ">" or code == "<=" or code == "<" or
          code == "!=" or code == "==");
}

bool IsUnaryOpreation(string code) {
  return (code == "+" or code == "-" or code == "!" or code == "~");
}

bool IsArithmaticStatement(const vector<string> &stmt) {
  return (stmt.size() == 5 and IsArithmaticOrLogicalOpreation(stmt[3]));
}

bool IsMemoryAccess(const vector<string> &stmt) {
  return (stmt.size() == 4 and stmt[2] == "*" and stmt[1] == "=");
}

bool IsAssignment(const vector<string> &stmt) {
  return (stmt.size() == 3 and stmt[1] == "=");
}

bool IsANumber(const string &token) {
  for (int i = (token[0] == '-' ? 1 : 0); i < token.length(); i++)
    if (not(token[i] >= '0' and token[i] <= '9'))
      return 0;
  return 1;
}

long long int Power(long long int x, long long int n) {
  long long int res = 1;
  while (n) {
    if (n & 1)
      res *= x;
    x = x * x;
    n >>= 1;
  }
  return res;
}

long long int ExtractNumber(string token) {
  bool isneg = token[0] == '-';
  if (isneg)
    token.erase(0, 1);
  long long int res = 0;
  long long int high = token.length() - 1;
  for (int i = 0; i < token.length(); i++)
    res += (token[i] - '0') * (Power(10, high - i));
  return (isneg ? -res : res);
}

void RemoveLastKTokens(vector<string> &stmt, int k) {
  if (k <= stmt.size()) {
    for (int i = 0; i < k; i++) {
      stmt.pop_back();
    }
  }
}

bool IsAPowerOf2(long long int number) {
  int cnt = 0;
  while (number > 0) {
    cnt += (number & 1);
    number >>= 1;
  }
  return cnt == 1;
}

int WhatPowerOf2(long long int num) {
  int power = 0;
  while (num > 0) {
    if (num & 1)
      return power;
    power += 1;
    num >>= 1;
  }
  return power;
}

long long int ConstantFoldingUtil(const long long int &num1,
                                  const long long int &num2, int Vopcode) {
  long long int res;
  bool b1 = IsAPowerOf2(num1);
  bool b2 = IsAPowerOf2(num2);
  switch (Vopcode) {
  case (ADD): {
    res = num1 + num2;
  } break;
  case (SUB): {
    res = num1 - num2;
  } break;
  case (MUL): {
    if (b1 or b2) {
      int pow = WhatPowerOf2((b1 ? num1 : num2));
      res = (num2 << pow);
    } else
      res = num1 * num2;
  } break;
  case (DIV): {
    if (num2 == 0)
      break;
    if (b2) {
      int pow = WhatPowerOf2(num2);
      res = (num1 >> pow);
    } else
      res = num1 / num2;
  } break;
  case (MOD): {
    if (num2 == 0)
      break;
    res = num1 % num2;
  } break;
  case (BITOR): {
    res = num1 | num2;
  } break;
  case (BITAND): {
    res = num1 & num2;
  } break;
  case (BITXOR): {
    res = num1 ^ num2;
  } break;
  case (OR): {
    res = num1 || num2;
  } break;
  case (AND): {
    res = num1 && num2;
  } break;
  case (RSHIFT): {
    res = (num1 >> num2);
  } break;
  case (LSHIFT): {
    res = (num1 << num2);
  } break;
  case (GREQ): {
    res = (num1 >= num2);
  } break;
  case (GR): {
    res = (num1 > num2);
  }
  case (LSEQ): {
    res = (num1 <= num2);
  } break;
  case (LS): {
    res = (num1 < num2);
  } break;
  case (NEQCMP): {
    res = (num1 != num2);
  } break;
  case (EQCMP): {
    res = (num1 == num2);
  } break;
  default:
    break;
  }
  return res;
}
