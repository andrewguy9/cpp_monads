/*
 * Monad Tools:
 * define monad typeclass
 */


#include<functional>
template <class T>
class MONAD {
  public:
};

#if 0
/* bind:: m A -> (A -> m B) -> m B */
template<class A, class B>
MONAD<B> monadBind(MONAD<A> m, std::function<MONAD<B>(A)> f);

/* return :: A -> Maybe A */
//TODO This is a special function for MAYBE, not general for all monads.
template<class A>
MONAD<A> return_(A j);
#endif

/*
 * define maybe typeclass
 */
template <class T>
class MAYBE : public MONAD<T> {

  template<typename A>
  friend MAYBE<A> Nothing();

  template<typename A>
  friend MAYBE<A> Just(A);

  template<class A>
  friend bool isNothing(MAYBE<A> m);

  template<class A>
  friend A fromJust(MAYBE<A> m);

  bool isNothing;
  T just;
};

/* Just a */
template<class T>
MAYBE<T> Just(T a)
{
  MAYBE<T> m;
  m.isNothing = false;
  m.just = a;
  return m;
}

/* maybe :: b -> (a -> b) -> Maybe a -> b */
template<class A, class B>
B maybe(B default_, B foo(A), MAYBE<A> m)
{
if (isJust(m)) {
    return foo(fromJust(m));
  } else {
    return default_;
  }
}

/* isJust :: Maybe a -> Bool */
template<class A>
bool isJust(MAYBE<A> m)
{
  return ! isNothing(m);
}

/* Nothing */
template<class T>
MAYBE<T> Nothing()
{
  MAYBE<T> m;
  m.isNothing = true;
  return m;
}

#include <stdexcept>
/* fromJust :: Maybe a -> a */
template<class A>
A fromJust(MAYBE<A> m)
{
  if (isJust(m)) {
    return m.just;
  } else {
    throw std::invalid_argument("fromJust called on Nothing");
  }
}

/* fromMaybe :: a -> Maybe a -> a */
template<class A>
A fromMaybe(A default_, MAYBE<A> m)
{
  if (isJust(m)) {
    return fromJust(m);
  } else {
    return default_;
  }
}

/* listToMaybe :: [a] -> Maybe a */
// Doesn't map to c++.
template <class A>
MAYBE<A> listToMaybe(std::vector<A> l)
{
  for (auto x: l) {
    return Just(x);
  }
  return Nothing<A>();
}

/* maybeToList :: Maybe a -> [a] */
// Doesn't map to c++.
template<class A>
std::vector<A> maybeToList(MAYBE<A> m)
{
  if (isJust(m)) {
    return std::vector<A>({fromJust(m)});
  } else {
    return std::vector<A>();
  }
}

#include <vector>
/* catMaybes :: [Maybe a] -> [a] */
template<class A>
std::vector<A> catMaybes(std::vector<MAYBE<A> > l)
{
  std::vector<A> o;
  for (auto x: l) {
    if (isJust(x)) {
      o.push_back(fromJust(x));
    }
  }
  return o;
}

//TODO make a lazy version of catMaybes.

/* mapMaybe :: (a -> Maybe b) -> [a] -> [b] */
template<class A, class B>
std::vector<B> mapMaybe(MAYBE<B> foo(A), std::vector<A> l)
{
  std::vector<B> o;
  for (auto x: l) {
    auto r = foo(x);
    if (isJust(r)) {
      o.push_back(fromJust(r));
    }
  }
  return o;
}
//TODO make a lazy version of mapMaybe.

template<class A>
bool operator ==(const MAYBE<A> &a, const MAYBE<A> &b)
{
  if (isNothing(a) and isNothing(b)) {
    return true;
  } else if (isJust(a) and isJust(b)) {
    return fromJust(a) == fromJust(b);
  } else {
    return false;
  }
}

template<class A>
bool operator <(const MAYBE<A> &a, const MAYBE<A> &b)
{
  if (isNothing(b)) {
    return false;
  } else if (isNothing(a)) {
    return true;
  } else {
    return fromJust(a) < fromJust(b);
  }
}

//TODO remimplemnt to be base def.
/* isNothing :: Maybe a -> Bool */
template<class A>
bool isNothing(MAYBE<A> m)
{
  return m.isNothing;
}

//TODO remimplemnt to be base def.
/* (>>=) :: Maybe a -> (a -> Maybe b) -> Maybe b */
template<class A, class B>
MAYBE<B> monadBind(MAYBE<A> m, std::function<MAYBE<B>(A)> f)
{
  if (isJust(m)) {
    return f(fromJust(m));
  } else {
    return Nothing<B>();
  }
}

/* return :: A -> Maybe A */
template<class A>
MAYBE<A> return_(A j)
{
  return Just(j);
}

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
void maybeTests() {
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

#include <string>
/* show :: Maybe a -> String */
template<class A>
string show(MAYBE<A> m)
{
  static string default_("Nothing");
  return maybe(default_, to_string, m);
}

/*
 * Safe Divide Application
 */

/* safe_divide :: int -> int -> Maybe int */
MAYBE<int> safe_divide(int a, int b)
{
  if (b == 0) {
    return Nothing<int>();
  } else {
    return Just(a/b);
  }
}

/* multiply :: int -> int -> Maybe int */
MAYBE<int> multiply(int a, int b)
{
  return Just(a * b);
}

MAYBE<int> add(int a, int b)
{
  return Just(a+b);
}

MAYBE<int> sub(int a, int b)
{
  return Just(a-b);
}

#include <iostream>
template<class A>
void printMaybe(MAYBE<A> m)
{
  cout << show(m) << endl;
}

class EXPR {
  public:
  virtual MAYBE<int> eval() = 0;
};

class VALUE : public EXPR
{
  int v;

  public:
  VALUE(int v_): v(v_) {}

  virtual MAYBE<int> eval()
  {
    return return_(v);
  }
};

class PLUS : public EXPR
{
  EXPR &x,&y;
  public:
  PLUS(EXPR &x_, EXPR &y_): x(x_), y(y_) {}

  /* eval :: EXPR -> Maybe Int */
  virtual MAYBE<int> eval()
  {
    //TODO there has to be a better way to write bind expressions.
    /*
     * eval (Div x y) = eval x >>= (\n->
     *    eval y >>= (\m -> safe_div n m))
     */
    auto &y_ = y;
    return monadBind(x.eval(), std::function<MAYBE<int>(int)> ([&y_](int n){
          return monadBind(y_.eval(), std::function<MAYBE<int>(int)> ([n](int m){
                return add(n,m); })); }));
  }
};

class SUB : public EXPR
{
  EXPR &x,&y;
  public:
  SUB(EXPR &x_, EXPR &y_): x(x_), y(y_) {}

  /* eval :: EXPR -> Maybe Int */
  virtual MAYBE<int> eval()
  {
    //TODO there has to be a better way to write bind expressions.
    /*
     * eval (Div x y) = eval x >>= (\n->
     *    eval y >>= (\m -> safe_div n m))
     */
    auto &y_ = y;
    return monadBind(x.eval(), std::function<MAYBE<int>(int)> ([&y_](int n){
          return monadBind(y_.eval(), std::function<MAYBE<int>(int)> ([n](int m){
                return sub(n,m); })); }));
  }
};

class DIV : public EXPR
{
  EXPR &x,&y;
  public:
  DIV(EXPR &x_, EXPR &y_): x(x_), y(y_) {}

  /* eval :: EXPR -> Maybe Int */
  virtual MAYBE<int> eval()
  {
    //TODO there has to be a better way to write bind expressions.
    /*
     * eval (Div x y) = eval x >>= (\n->
     *    eval y >>= (\m -> safe_div n m))
     */
    auto &y_ = y;
    return monadBind(x.eval(), std::function<MAYBE<int>(int)> ([&y_](int n){
          return monadBind(y_.eval(), std::function<MAYBE<int>(int)> ([n](int m){
                return safe_divide(n,m); })); }));
  }
};

class MULT : public EXPR
{
  EXPR &x,&y;
  public:
  MULT(EXPR &x_, EXPR &y_): x(x_), y(y_) {}
  /* eval :: EXPR -> Maybe Int */
  virtual MAYBE<int> eval()
  {
    //TODO there has to be a better way to write bind expressions.
    /*
     * eval (Div x y) = eval x >>= (\n->
     *    eval y >>= (\m -> safe_div n m))
     */
    auto &y_ = y;
    return monadBind(x.eval(), std::function<MAYBE<int>(int)> ([&y_](int n){
          return monadBind(y_.eval(), std::function<MAYBE<int>(int)> ([n](int m){
                return multiply(n,m); })); }));
  }
};

class NEG: public EXPR
{
  EXPR &x;
  public:
  NEG(EXPR &x_): x(x_) {}

  /* eval :: EXPR -> Maybe Int */
  virtual MAYBE<int> eval()
  {
    auto n = VALUE(-1);
    return MULT(n, x).eval();
  }
};

int main() {
  maybeTests();
  assert (2 == fromJust(safe_divide(5,2)));

  auto five = VALUE(5);
  auto two  = VALUE(2);
  auto zero = VALUE(0);

  auto five_over_two = DIV(five, two);
  assert (2 == fromJust(five_over_two.eval()));
  auto two_over_zero = DIV(two, zero);
  assert (isNothing(two_over_zero.eval()));

  auto five_over_two_over_zero = DIV(five, two_over_zero);
  assert (isNothing(five_over_two_over_zero.eval()));
  auto two_over_zero_over_five = DIV(two_over_zero, five);
  assert (isNothing(two_over_zero_over_five.eval()));
  auto zero_over_five_over_two = DIV(zero, five_over_two);
  assert (0 == fromJust(zero_over_five_over_two.eval()));

  auto five_mult_two = MULT(five,two);
  assert (10 == fromJust(five_mult_two.eval()));
  auto zero_mult_two = MULT(zero, two);
  assert (0 == fromJust(zero_mult_two.eval()));
  auto two_over_zero_mult_five = MULT(two_over_zero, five);
  assert (isNothing(two_over_zero_mult_five.eval()));

  auto neg_five = NEG(five);
  assert (-5 == fromJust(neg_five.eval()));
  auto neg_two_over_zero = NEG(two_over_zero);
  assert (isNothing(neg_two_over_zero.eval()));

  auto five_plus_two = PLUS(five,two);
  assert (7 == fromJust(five_plus_two.eval()));
  auto five_sub_two = SUB(five, two);
  assert (3 == fromJust(five_sub_two.eval()));
}
