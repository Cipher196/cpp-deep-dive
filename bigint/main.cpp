#include "my_std.h"
#include "bigint.hpp"


int main() {
  string s;
  cin >> s;
  bigint bi;
  bi.from_string(s);
  cout << bi.to_string();
  
  bigint ci("39399kkk2i2i2kkk2kk2k");
  cout<<'\n'<<ci.to_string();
  return 0;
}
