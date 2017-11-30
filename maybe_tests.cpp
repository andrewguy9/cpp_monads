#include "maybe.hpp"
/*
 * maybeTests for maybe typeclass
 */
using namespace std;

int inc (int a) { return ++a; }

MAYBE<int> Even(int a) {
  if (a%2==0) {
    return Just(a);
  } else {
    return Nothing<int>();
  }
}

#include<assert.h>
int main()
{
  Just(1);
  Nothing<int>();

  int x = maybe(0, inc, Just(5));
  assert (6 == x);

  int y = maybe(0, inc, Nothing<int>());
  assert (0 == y);

  assert (false == isJust(Nothing<int>()));
  assert (isJust(Just(5)));

  assert (5 == fromJust(Just(5)));

  try {
    fromJust(Nothing<int>());
    throw;
  } catch (const std::invalid_argument& ia) {
  }

  assert (1 == fromMaybe(0, Just(1)));
  assert (0 == fromMaybe(0, Nothing<int>()));

  vector<int> l = {1, 2, 3, 4, 5};
  vector<int> empty;

  assert (isNothing(listToMaybe(empty)));
  assert (1 == fromJust(listToMaybe(l)));

  auto maybeToListEmpty = maybeToList(Nothing<int>());
  assert (maybeToListEmpty.empty());

  auto maybeToListFull = maybeToList(Just(5));
  std::vector<int> list5 = {5};
  assert (list5 == maybeToListFull);

  vector<int> evens = mapMaybe(Even, l);
  vector<int> check_evens = {2,4};
  assert (check_evens == evens);

  vector<MAYBE<int> > ml = {Just(1), Nothing<int>(), Just(2), Nothing<int>(), Just(3)};
  vector<int> justs = catMaybes(ml);
  vector<int> check_justs = {1,2,3};
  assert (check_justs == justs);

  assert(Nothing<int>() == Nothing<int>());
  assert(!(Nothing<int>() == Just(1)));
  assert(!(Just(1) == Nothing<int>()));
  assert(Just(1) == Just(1));
  assert(! (Just(1) == Just(2)));

  assert(!(Nothing<int>() < Nothing<int>()));
  assert(!(Just(1) < Nothing<int>()));
  assert(Nothing<int>() < Just(1));
  assert(!(Just(1) < Just(0)));
  assert(!(Just(1) < Just(0)));
  assert(Just(1) < Just(2));
}
