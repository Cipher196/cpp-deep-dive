#include "arithmetic.hpp"


int main() {
  string a, b;
  cout<<"Please provide first number to add: ";
  cin>>a;
  cout<<"Please provide second number to add: ";
  cin>>b;

  bigint x(a);
  bigint y(b);

  bigint z=subtract(x,y);

  string c=z.to_string();

  cout<<"Their sum is "<<c;
  return 0;
}
