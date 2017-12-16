#ifndef CALC_H
#define CALC_H

#include "maybe.hpp"

/* safe_divide :: int -> int -> Maybe int */
Maybe<int> safe_divide(int a, int b)
{
  if (b == 0) {
    return Nothing<int>();
  } else {
    return Just(a/b);
  }
}

/* neg :: int -> Maybe int */
Maybe<int> neg(int a) {
    return Just<int>(-1 * a);
}

/* multiply :: int -> int -> Maybe int */
Maybe<int> multiply(int a, int b)
{
  return Just(a * b);
}

Maybe<int> add(int a, int b)
{
  return Just(a + b);
}

Maybe<int> sub(int a, int b)
{
  return Just(a - b);
}

class Expr {
  public:
  virtual Maybe<int> eval() const = 0;
};

class Value : public Expr {
  const int v;

  friend bool operator==(const Value & a, const Value & b);

  public:
  Value(): v(0) {} //GRRRR
  Value(int v_): v(v_) {}

  virtual Maybe<int> eval() const {
    return monadReturn(v);
  }
};

bool operator==(const Value & a, const Value & b) {
  return a.v == b.v;
}

Value Value_(int v) {
  return Value(v);
}

class Plus : public Expr
{
  const Expr &x,&y;
  public:
  Plus(Expr &x_, Expr &y_): x(x_), y(y_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval() const {
    return join<int>(liftM2<int,int,Maybe<int> >(add, x.eval(), y.eval()));
  }
};

Plus Plus_(Expr & x, Expr & y) {
  return Plus(x,y);
}

class Sub : public Expr
{
  const Expr &x,&y;
  public:
  Sub(Expr & x_, Expr & y_): x(x_), y(y_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval() const {
    return join<int>(liftM2<int,int,Maybe<int> >(sub, x.eval(), y.eval()));
  }
};

Sub Sub_(Expr & x, Expr & y) {
  return Sub(x,y);
}

class Div : public Expr
{
  const Expr &x,&y;
  public:
  Div(Expr & x_, Expr & y_): x(x_), y(y_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval() const {
    return join<int>(liftM2<int,int,Maybe<int> >(safe_divide, x.eval(), y.eval()));
  }
};

Div Div_(Expr & x, Expr & y) {
  return Div(x,y);
}

class Mult : public Expr
{
  const Expr &x,&y;
  public:
  Mult(Expr & x_, Expr & y_): x(x_), y(y_) {}
  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval() const {
    return join<int>(liftM2<int,int,Maybe<int> >(multiply, x.eval(), y.eval()));
  }
};

Mult Mult_(Expr & x, Expr & y) {
  return Mult(x,y);
}

class Neg: public Expr
{
  const Expr &x;
  public:
  Neg(Expr &x_): x(x_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval() const {
    return monadBind<int,int>(x.eval(), neg);
  }
};

Neg Neg_(Expr & x) {
  return Neg(x);
}

#include<string>
#include<utility>
Maybe<int> readMaybeInt(const std::string & s) {
  size_t idx;
  try {
    int i(stoi(s, &idx));
    if (idx != s.length()) {
      return Nothing<int>();
    } else {
      return Just(i);
    }
  } catch (std::invalid_argument) {
    return Nothing<int>();
  }
}

Maybe<Value> tryParseValue(std::string s) {
  return fmap(Value_, readMaybeInt(s));
}

#endif
