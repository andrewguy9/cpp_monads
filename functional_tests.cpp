#include "functional.hpp"
/*
 * maybeTests for maybe typeclass
 */
using namespace std;

int add(int a, int b) { return a+b; }
int inc(int a) { return a+1; }
int divide(int a, int b) { return a/b; }

#include<assert.h>
#include<vector>
int main()
{
  auto divide_uncurry = uncurry<int,int,int>(divide);
  assert(divide_uncurry(pair<int,int>(8,2)) == divide(8,2));

  auto divide_restored = curry<int,int,int>(divide_uncurry);
  assert(4 == divide_restored(8)(2));

  vector<int> nums = {1,2,3};
  vector<int> inced = {2,3,4};
  vector<int> map_test = map<int,int>(inc, nums);
  assert(map_test == inced);

  auto sum = foldl<int,int>(add, 0, nums);
  assert( 6 == sum );

  return 0;
}
