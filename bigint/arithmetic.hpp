// arithmetic.hpp
#pragma once

#include "bigint.hpp"

// Value-based magnitude comparison helper (|a| >= |b|)
inline bool abs_geq(const bigint &a, const bigint &b) {
  if (a.get_digits().size() != b.get_digits().size()) {
    return a.get_digits().size() > b.get_digits().size();
  }
  for (long long i = (long long)a.get_digits().size() - 1; i >= 0; --i) {
    if (a.get_digits()[i] != b.get_digits()[i]) {
      return a.get_digits()[i] > b.get_digits()[i];
    }
  }
  return true; 
}

// Ensures a zero result doesn't accidentally retain a negative sign
inline void sanitize_zero_sign(bigint &n) {
  if (n.get_digits().size() == 1 && n.get_digits()[0] == 0) {
    if (!n.ip()) n.flipSign();
  }
}

// Pure magnitude addition: Computes |a| + |b| (Assumes |a| >= |b|)
inline bigint bigint_add(const bigint &a, const bigint &b) {
  bigint c;
  c.get_digit().clear();
  
  unsigned __int128 carry = 0;
  size_t a_size = a.get_digits().size();
  size_t b_size = b.get_digits().size();

  for (size_t i = 0; i < a_size; i++) {
    unsigned __int128 sum = (unsigned __int128)a.get_digits()[i] + carry;
    if (i < b_size) {
      sum += b.get_digits()[i];
    }
    c.get_digit().push_back((uint64_t)sum);
    carry = sum >> 64; // Captures upper 64-bit carry cleanly
  }

  if (carry) {
    c.get_digit().push_back((uint64_t)carry);
  }
  return c;
}

// Pure magnitude subtraction: Computes |a| - |b| (Assumes |a| >= |b|)
inline bigint bigint_sub(const bigint &a, const bigint &b) {
  bigint c;
  c.get_digit().clear();

  uint64_t borrow = 0;
  size_t a_size = a.get_digits().size();
  size_t b_size = b.get_digits().size();

  for (size_t i = 0; i < a_size; i++) {
    uint64_t b_val = (i < b_size) ? b.get_digits()[i] : 0;
    unsigned __int128 s = (unsigned __int128)a.get_digits()[i] - b_val - borrow;
    c.get_digit().push_back((uint64_t)s);
    borrow = (s >> 64) ? 1 : 0; // If underflow occurs, bit 64+ clears to 1s
  }

  // Pop trailing zeroes to prevent leading zero string outputs like "0001"
  while (c.get_digits().size() > 1 && c.get_digits().back() == 0) {
    c.get_digit().pop_back();
  }
  return c;
}

// Global signed addition handler
inline bigint add(const bigint &a, const bigint &b) {
  bigint ans;
  if (a.ip() == b.ip()) {
    // Both positive or both negative
    ans = abs_geq(a, b) ? bigint_add(a, b) : bigint_add(b, a);
    if (!a.ip()) ans.flipSign();
  } else {
    // Mixed signs: addition turns into subtraction rules
    if (abs_geq(a, b)) {
      ans = bigint_sub(a, b);
      if (!a.ip()) ans.flipSign();
    } else {
      ans = bigint_sub(b, a);
      if (!b.ip()) ans.flipSign();
    }
  }
  sanitize_zero_sign(ans);
  return ans;
}

// Global signed subtraction handler
inline bigint subtract(const bigint &a, const bigint &b) {
  bigint ans;
  if (a.ip() != b.ip()) {
    // Mixed signs: A - (-B) -> A + B  or  (-A) - B -> -(A + B)
    ans = abs_geq(a, b) ? bigint_add(a, b) : bigint_add(b, a);
    if (!a.ip()) ans.flipSign();
  } else {
    // Same signs: A - B or (-A) - (-B) -> B - A
    if (abs_geq(a, b)) {
      ans = bigint_sub(a, b);
      if (!a.ip()) ans.flipSign();
    } else {
      ans = bigint_sub(b, a);
      if (a.ip()) ans.flipSign(); // e.g. 10 - 20 = -10
    }
  }
  sanitize_zero_sign(ans);
  return ans;
}
