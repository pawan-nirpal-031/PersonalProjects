typedef long long int ll;
typedef long double ld;

class Utilities {
private:
public:
  bool IsAssociativeOpreation(string op) {
    return (op == "+" or op == "*" or op == "|" or op == "&" or op == "||" or
            op == "&&" or op == "^");
  }

  bool IsANumber(string &token) {
    for (char c : token)
      if (not(c >= '0' and c <= '9'))
        return 0;
    return 1;
  }

  ll Power(ll x, ll n) {
    ll res = 1;
    while (n) {
      if (n & 1)
        res *= x;
      x = x * x;
      n >>= 1;
    }
    return res;
  }

  ll ExtractNumber(string token) {
    ll res = 0;
    int high = token.length() - 1;
    for (int i = 0; i < token.length(); i++)
      res += (token[i] - '0') * (Power(10, high - i));
    return res;
  }

  bool IsAPowerOf2(ll number) {
    int cnt = 0;
    while (number > 0) {
      cnt += (number & 1);
      number >>= 1;
    }
    return cnt == 1;
  }

  int WhatPowerOf2(ll num) {
    int power = 0;
    while (num > 0) {
      if (num & 1)
        return power;
      power += 1;
      num >>= 1;
    }
    return power;
  }
};
