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

  virtual Maybe<int> eval()
  {
    return monadReturn(v);
  }
};

class Plus : public Expr
{
  Expr &x,&y;
  public:
  Plus(Expr &x_, Expr &y_): x(x_), y(y_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval()
  {
    //TODO there has to be a better way to write bind expressions.
    /*
     * eval (Div x y) = eval x >>= (\n->
     *    eval y >>= (\m -> safe_div n m))
     */
    auto &y_ = y;
    return monadBind(x.eval(), std::function<Maybe<int>(int)> ([&y_](int n){
          return monadBind(y_.eval(), std::function<Maybe<int>(int)> ([n](int m){
                return add(n,m); })); }));
  }
};

class Sub : public Expr
{
  Expr &x,&y;
  public:
  Sub(Expr &x_, Expr &y_): x(x_), y(y_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval()
  {
    //TODO there has to be a better way to write bind expressions.
    /*
     * eval (Div x y) = eval x >>= (\n->
     *    eval y >>= (\m -> safe_div n m))
     */
    auto &y_ = y;
    return monadBind(x.eval(), std::function<Maybe<int>(int)> ([&y_](int n){
          return monadBind(y_.eval(), std::function<Maybe<int>(int)> ([n](int m){
                return sub(n,m); })); }));
  }
};

class Div : public Expr
{
  Expr &x,&y;
  public:
  Div(Expr &x_, Expr &y_): x(x_), y(y_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval()
  {
    //TODO there has to be a better way to write bind expressions.
    /*
     * eval (Div x y) = eval x >>= (\n->
     *    eval y >>= (\m -> safe_div n m))
     */
    auto &y_ = y;
    return monadBind(x.eval(), std::function<Maybe<int>(int)> ([&y_](int n){
          return monadBind(y_.eval(), std::function<Maybe<int>(int)> ([n](int m){
                return safe_divide(n,m); })); }));
  }
};

class Mult : public Expr
{
  Expr &x,&y;
  public:
  Mult(Expr &x_, Expr &y_): x(x_), y(y_) {}
  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval()
  {
    //TODO there has to be a better way to write bind expressions.
    /*
     * eval (Div x y) = eval x >>= (\n->
     *    eval y >>= (\m -> safe_div n m))
     */
    auto &y_ = y;
    return monadBind(x.eval(), std::function<Maybe<int>(int)> ([&y_](int n){
          return monadBind(y_.eval(), std::function<Maybe<int>(int)> ([n](int m){
                return multiply(n,m); })); }));
  }
};

class Neg: public Expr
{
  Expr &x;
  public:
  Neg(Expr &x_): x(x_) {}

  /* eval :: Expr -> Maybe Int */
  virtual Maybe<int> eval()
  {
    auto n = Value(-1);
    return Mult(n, x).eval();
  }
};

#endif
