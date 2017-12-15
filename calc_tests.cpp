#include "calc.hpp"

#include<assert.h>

int main() {
  assert (readMaybeInt("1") == Just(1));
  assert (readMaybeInt("x") == Nothing<int>());
  assert (readMaybeInt("x1") == Nothing<int>());
  assert (readMaybeInt("1x") == Nothing<int>());

  assert (2 == fromJust(safe_divide(5,2)));

  auto five = Value(5);
  auto two  = Value(2);
  auto zero = Value(0);

  auto five_over_two = Div(five, two);
  assert (2 == fromJust(five_over_two.eval()));
  auto two_over_zero = Div(two, zero);
  assert (isNothing(two_over_zero.eval()));

  auto five_over_two_over_zero = Div(five, two_over_zero);
  assert (isNothing(five_over_two_over_zero.eval()));
  auto two_over_zero_over_five = Div(two_over_zero, five);
  assert (isNothing(two_over_zero_over_five.eval()));
  auto zero_over_five_over_two = Div(zero, five_over_two);
  assert (0 == fromJust(zero_over_five_over_two.eval()));

  auto five_mult_two = Mult(five,two);
  assert (10 == fromJust(five_mult_two.eval()));
  auto zero_mult_two = Mult(zero, two);
  assert (0 == fromJust(zero_mult_two.eval()));
  auto two_over_zero_mult_five = Mult(two_over_zero, five);
  assert (isNothing(two_over_zero_mult_five.eval()));

  auto neg_five = Neg(five);
  assert (-5 == fromJust(neg_five.eval()));
  auto neg_two_over_zero = Neg(two_over_zero);
  assert (isNothing(neg_two_over_zero.eval()));

  auto five_plus_two = Plus(five,two);
  assert (7 == fromJust(five_plus_two.eval()));
  auto five_sub_two = Sub(five, two);
  assert (3 == fromJust(five_sub_two.eval()));

  return 0;
}
