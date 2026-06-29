#pragma once

#include "my_std.h"

class bigint {
private:
  bool isPositive = true;
  // we are user base = uint64_t ( 2^64 )
  vector<uint64_t> digits;
  // A = digit[0] + digit[1]*base^1 + ....

  void multiply_add(uint64_t scalar, uint64_t add_value) {
    unsigned __int128 carry = add_value;

    for (size_t i = 0; i < digits.size(); i++) {
      unsigned __int128 curr = (unsigned __int128)digits[i] * scalar + carry;
      digits[i] = (uint64_t)curr;
      carry = curr >> 64;
    }

    while (carry > 0) {
      digits.push_back((uint64_t)carry);
      carry >>= 64;
    }
  }

  uint64_t divide_scalar(uint64_t divisor) {
    unsigned __int128 carry = 0;

    for (long long i = digits.size() - 1; i >= 0; --i) {
      unsigned __int128 curr = (carry << 64) | digits[i];
      digits[i] = (uint64_t)(curr / divisor);
      carry = curr % divisor;
    }

    while (digits.size() > 1 && digits.back() == 0) {
      digits.pop_back();
    }

    return (uint64_t)carry;
  }

  void zero_check() {
    if (digits.empty() || (digits.size() == 1 && digits[0] == 0)) {
      digits = {0};
      isPositive = true;
    }
  }

public:
  bool ip() const { return isPositive; }

  void flipSign() {
    if (!(digits.size() == 1 && digits[0] == 0)) {
      isPositive = !isPositive;
    }
  }

  const vector<uint64_t> &get_digits() const { return digits; }

  vector<uint64_t> &get_digit() { return digits; }

  void at(size_t i, uint64_t val) { digits[i] = val; }
  // Inside your bigint class
  bigint shift_left(size_t blocks) const {
    if (blocks == 0 || (digits.size() == 1 && digits[0] == 0)) {
      return *this;
    }

    bigint result;
    result.digits.assign(blocks,
                         0); // Blindly pad with exact number of required 0s

    // Append the actual numbers on top of the shifted blocks
    for (size_t i = 0; i < this->digits.size(); i++) {
      result.digits.push_back(this->digits[i]);
    }

    result.isPositive = this->isPositive;
    return result;
  }
  bigint(long long val = 0) {
    uint64_t absVal;
    if (val < 0) {
      isPositive = false;
      absVal = static_cast<uint64_t>(-(val + 1)) + 1;
    } else {
      isPositive = true;
      absVal = (uint64_t)val;
    }

    digits.push_back(absVal);
    zero_check();
  }

  bigint(const std::string &s) { from_string(s); }

  bigint(vector<uint64_t> v, bool sign) {
    digits = v;
    isPositive = sign;
  }

  void from_string(const string &s) {
    digits.clear();
    isPositive = true;
    if (s.empty())
      return;

    size_t start = 0;
    if (s[0] == '-') {
      isPositive = false;
      start = 1;
    } else if (s[0] == '+') {
      start = 1;
    }

    digits.push_back(0);

    for (size_t i = start; i < s.length(); i++) {
      if (s[i] < '0' || s[i] > '9')
        continue;
      uint64_t digit_val = s[i] - '0';
      multiply_add(10, digit_val);
    }
    zero_check();
  }

  string to_string() const {
    if (digits.empty() || (digits.size() == 1 && digits[0] == 0)) {
      return "0";
    }

    bigint temp = *this;
    string result = "";

    while (!(temp.digits.size() == 1 && temp.digits[0] == 0)) {
      uint64_t remainder = temp.divide_scalar(10);
      result += std::to_string(remainder);
    }

    if (!isPositive) {
      result += "-";
    }

    reverse(result.begin(), result.end());
    return result;
  }
};
