#include "maybe.hpp"
/*
 * maybeTests for maybe typeclass
 */
using namespace std;

int inc (int a) { return ++a; }
int add (int a, int b) { return a + b; }

Maybe<int> Even(int a) {
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

  vector<Maybe<int> > ml = {Just(1), Nothing<int>(), Just(2), Nothing<int>(), Just(3)};
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

  Maybe<int> one = Just(1);
  Maybe<int> two = Just(2);
  Maybe<int> three = Just(3);
  Maybe<int> nothing = Nothing<int>();
  assert(monadPlus(one,two) == one);
  assert(monadPlus(nothing, two) == two);
  assert(monadPlus(one, nothing) == one);
  assert(monadPlus(nothing, nothing) == nothing);

  auto liftm2_1 = liftM2<int,int,int>(add, one, two);
  assert( liftm2_1 == three);
  auto liftm2_2 = liftM2<int,int,int>(add, nothing, two);
  assert( liftm2_2 == nothing);
  auto liftm2_3 = liftM2<int,int,int>(add, one, nothing);
  assert( liftm2_3 == nothing);
  auto liftm2_4 = liftM2<int,int,int>(add, nothing, nothing);
  assert( liftm2_4 == nothing);

  Maybe< Maybe<int> > mmone = Just(Just(1));
  Maybe<int> mone = Just(1);
  assert(join(mmone) == mone);
  Maybe<int> ni = Nothing<int>();
  Maybe<Maybe<int> > nni = Nothing< Maybe<int> >();
  assert(join(nni) == ni);
}
