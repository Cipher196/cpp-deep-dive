#pragma once

#include "bigint.hpp"

inline bigint bigint_add(const bigint &a, const bigint &b) {
  bigint c(0);
  unsigned __int128 s = a.get_digits()[0] + b.get_digits()[0];
  c.get_digit()[0] = (uint64_t)s;
  uint64_t d = (uint64_t)(s >> 64);

  for (size_t i = 0; i < b.get_digits().size(); i++) {
    s = a.get_digits()[i] + b.get_digits()[i] + d;
    d = (uint64_t)(s >> 64);
    c.get_digit().push_back((uint64_t)s);
  }

  for (size_t i = b.get_digits().size(); i < a.get_digits().size(); i++) {
    s = a.get_digits()[i] + d;
    d = (uint64_t)(s >> 64);
    c.get_digit().push_back((uint64_t)s);
  }

  if (d)
    c.get_digit().push_back(d);
  return c;
}

inline bigint add(const bigint &a, const bigint &b) {
  bigint ans;
  if (a.ip() && b.ip()) {
    if (a.get_digits().size() >= b.get_digits().size()) {
      ans = bigint_add(a, b);
    } else {
      ans = bigint_add(b, a);
    }
    return ans;
  } else if (!a.ip() && !b.ip()) {
    if (a.get_digits().size() >= b.get_digits().size()) {
      ans = bigint_add(a, b);
    } else {
      ans = bigint_add(b, a);
    }
    ans.flipSign();
    return ans;
  } else {
    // TODO:  Implement substract functionality
    return ans;
  }
}
