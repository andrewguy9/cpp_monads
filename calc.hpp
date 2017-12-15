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
  virtual Maybe<int> eval() = 0;
};

class Value : public Expr {
  int v;

  public:
  Value(int v_): v(v_) {}

  virtual Maybe<int> eval() {
    return monadReturn(v);
  }
};

const Value Value_(int v) {
  return Value(v);
}

class Plus : public Expr
{
  Expr &x,&y;
  public:
  Plus(Expr &x_, Expr &y_): x(x_), y(y_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval() {
    return join<int>(liftM2<int,int,Maybe<int> >(add, x.eval(), y.eval()));
  }
};

const Plus Plus_(Expr &x, Expr &y) {
  return Plus(x,y);
}

class Sub : public Expr
{
  Expr &x,&y;
  public:
  Sub(Expr &x_, Expr &y_): x(x_), y(y_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval() {
    return join<int>(liftM2<int,int,Maybe<int> >(sub, x.eval(), y.eval()));
  }
};

const Sub Sub_(Expr &x, Expr &y) {
  return Sub(x,y);
}

class Div : public Expr
{
  Expr &x,&y;
  public:
  Div(Expr &x_, Expr &y_): x(x_), y(y_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval() {
    return join<int>(liftM2<int,int,Maybe<int> >(safe_divide, x.eval(), y.eval()));
  }
};

const Div Div_(Expr &x, Expr &y) {
  return Div(x,y);
}

class Mult : public Expr
{
  Expr &x,&y;
  public:
  Mult(Expr &x_, Expr &y_): x(x_), y(y_) {}
  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval() {
    return join<int>(liftM2<int,int,Maybe<int> >(multiply, x.eval(), y.eval()));
  }
};

const Mult Mult_(Expr &x, Expr &y) {
  return Mult(x,y);
}

class Neg: public Expr
{
  Expr &x;
  public:
  Neg(Expr &x_): x(x_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval() {
    return monadBind<int,int>(x.eval(), neg);
  }
};

Neg Neg_(Expr &x) {
  return Neg(x);
}

#endif
