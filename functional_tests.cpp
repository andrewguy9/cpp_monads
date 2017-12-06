#include "functional.hpp"
/*
 * maybeTests for maybe typeclass
 */
using namespace std;

int divide(int a, int b) { return a/b; }

#include<assert.h>
int main()
{
  auto divide_uncurry = uncurry<int,int,int>(divide);
  assert(divide_uncurry(pair<int,int>(8,2)) == divide(8,2));
  return 0;
}
