#include "maybe.hpp"
#include <string>

using namespace std;

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
    return monadReturn_(v);
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

#include<assert.h>
int main() {
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

  return 0;
}
