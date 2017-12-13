#include"bool.hpp"

/* Tests */
using namespace std;
#include<iostream>
#include<assert.h>
int main() {
  auto t = True();
  auto f = False();

  assert(True());
  assert(!False());

  assert(!(t != t));
  assert(t != f);
  assert(f != t);
  assert(!(f != f));

  assert(t && t);
  assert(!(t && f));
  assert(!(f && t));
  assert(!(f && f));

  assert(t || t);
  assert(t || f);
  assert(f || t);
  assert(!(f || f));

  assert(! bool_(0, 1, False()));
  assert(bool_(0, 1, True()));
}
