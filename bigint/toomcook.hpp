#include "addition_and_subtraction.hpp"
#include "bigint.hpp"
#include "karatsuba.hpp"

inline int toom_threshold = 30;

inline bigint scalar_divide(const bigint &a, unsigned __int128 b) {
  bigint c = a;
  vector<uint64_t> &v = c.get_digit();
  unsigned __int128 k = 0;
  for (size_t i = v.size() - 1; i >= 0; i--) {
    k = (k << 64) + v[i];
    v[i] = (uint64_t)k / b;
    k = k % b;
    if (!i)
      break;
  }
  if (!a.ip())
    c.flipSign();

  while (c.get_digits().size() > 1 && c.get_digits().back() == 0) {
    c.get_digit().pop_back();
  }

  return c;
}

inline bigint place_in_front(const bigint &c,
                             const bigint &v) { // assuming v is positive
  bigint k = v;
  for (size_t i = 0; i < c.get_digits().size(); i++) {
    k.get_digit().push_back(c.get_digits()[i]);
  }
  if (!c.ip())
    k.flipSign();
  return k;
}

inline bigint multiply_toom_cook(const bigint &a, const bigint &b);

inline bigint multiply_toom_cook_core(const bigint &a, const bigint &b) {
  size_t l = std::max(a.get_digits().size(), b.get_digits().size());
  if (l < toom_threshold) {
    return multiply_karatsuba(a, b);
  }

  size_t k = l / 3;

  bigint a0 = mod_with_basePower_k(a, k);
  bigint aa = div_with_basePower_k(a, k);
  bigint a1 = mod_with_basePower_k(aa, k);
  bigint a2 = div_with_basePower_k(aa, k);

  bigint b0 = mod_with_basePower_k(b, k);
  bigint bb = div_with_basePower_k(b, k);
  bigint b1 = mod_with_basePower_k(bb, k);
  bigint b2 = div_with_basePower_k(bb, k);

  bigint a02 = add(a0, a2);
  bigint b02 = add(b0, b2);

  bigint v0 = multiply_toom_cook(a0, b0);
  bigint v1 = multiply_toom_cook(add(a02, a1), add(b02, b1));
  bigint v11 = multiply_toom_cook(subtract(a02, a1), subtract(b02, b1));
  bigint v2 = multiply_toom_cook(
      add(add(a0, scalar_multiply(a1, 2)), scalar_multiply(a2, 4)),
      add(add(b0, scalar_multiply(b1, 2)), scalar_multiply(b2, 4)));
  bigint vinf = multiply_toom_cook(a2, b2);

  bigint t11 = add(add(scalar_multiply(v0, 3), scalar_multiply(v11, 2)), v2);
  bigint t22 = add(v1, v11);

  bigint t1 = scalar_divide(t11, 6);
  bigint t2 = scalar_divide(t22, 2);

  bigint coeff0 = v0;
  bigint coeff1 = subtract(v1, t1);
  bigint coeff2 = subtract(t2, add(v0, vinf));
  bigint coeff3 = subtract(t1, t2);
  bigint coeff4 = vinf;

  bigint c = coeff0;
  c = add(c, coeff1.shift_left(k));
  c = add(c, coeff2.shift_left(2 * k));
  c = add(c, coeff3.shift_left(3 * k));
  c = add(c, coeff4.shift_left(4 * k));

  return c;
}

inline bigint multiply_toom_cook(const bigint &a, const bigint &b) {
  bigint abs_a = a;
  if (!abs_a.ip())
    abs_a.flipSign();

  bigint abs_b = b;
  if (!abs_b.ip())
    abs_b.flipSign();

  bigint result = multiply_toom_cook_core(abs_a, abs_b);

  if (a.ip() ^ b.ip())
    result.flipSign();
  return result;
}
