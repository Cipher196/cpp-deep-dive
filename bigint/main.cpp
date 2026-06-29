#include "bigint.hpp"
#include "toomcook.hpp"
#include <chrono>
#include <random>

int main() {
  int n = 21;
  cout << "Please provide a value to n: ";
  cin >> n;
  int k = 1e9;

  bigint c(rand() % k);
  n--;

  while (n--)
    c.get_digit().push_back(rand() % k);

  string sc = c.to_string();
  cout << "c = " << sc << '\n';

  auto t1 = std::chrono::high_resolution_clock::now();
  bigint a = multiply_naive(c, c);
  auto t2 = std::chrono::high_resolution_clock::now();

  float time1 =
      float(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)
                .count()) /
      1000;

  auto t3 = std::chrono::high_resolution_clock::now();
  bigint b = multiply_karatsuba(c, c);
  auto t4 = std::chrono::high_resolution_clock::now();

  auto time2 =
      float(std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3)
                .count()) /
      1000;

  auto t5 = std::chrono::high_resolution_clock::now();
  bigint d = multiply_toom_cook(c, c);
  auto t6 = std::chrono::high_resolution_clock::now();

  auto time3 =
      float(std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5)
                .count()) /
      1000;

  cout << "\nWe are computing square of a number which has " << n
       << " digits.\n";

  cout << "\nTime taken by naive multiply : " << time1 << " Sec\n\n";

  cout << "Time taken by karatsuba multiply : " << time2 << " Sec\n\n";

  cout << "Time taken by toom cook multiply : " << time3 << " Sec\n";

  return 0;
}
