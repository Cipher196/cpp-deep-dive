#include "my_std.h"
#include <iostream>
#include <string>

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

    // We go from most significant digit to least significant
    for (long long i = digits.size() - 1; i >= 0; --i) {
      unsigned __int128 current = (carry << 64) | digits[i];
      digits[i] = (uint64_t)(current / divisor);
      carry = current % divisor;
    }

    // Remove leading zeros resulting from division
    while (digits.size() > 1 && digits.back() == 0) {
      digits.pop_back();
    }

    return (uint64_t)carry; // This is our decimal digit
  }

public:
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
  }
  string to_string() const {
    if (digits.empty() || (digits.size() == 1 && digits[0] == 0)) {
      return "0";
    }

    // We make a copy because division will destroy the number
    bigint temp = *this;
    string result = "";

    while (!(temp.digits.size() == 1 && temp.digits[0] == 0)) {
      uint64_t remainder = temp.divide_scalar(10);
      result += std::to_string(remainder);
    }

    if (!isPositive) {
      result += "-";
    }

    // Since we extracted digits from right to left, we must reverse the string
    reverse(result.begin(), result.end());
    return result;
  }
};

int main() {
  string s;
  std::cin >> s;
  bigint bi;
  bi.from_string(s);
  std::cout << bi.to_string();

  return 0;
}
