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

class EXPR {
  public:
  virtual Maybe<int> eval() = 0;
};

class VALUE : public EXPR
{
  int v;

  public:
  VALUE(int v_): v(v_) {}

  virtual Maybe<int> eval()
  {
    return monadReturn_(v);
  }
};

/*
VALUE Value(int v) {
  return Value(v);
}
*/

class PLUS : public EXPR
{
  EXPR &x,&y;
  public:
  PLUS(EXPR &x_, EXPR &y_): x(x_), y(y_) {}

  /* eval :: EXPR -> Maybe Int */
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

/*
PLUS Plus(EXPR &x, EXPR &y) {
  return PLUS(x, y);
}
*/

class SUB : public EXPR
{
  EXPR &x,&y;
  public:
  SUB(EXPR &x_, EXPR &y_): x(x_), y(y_) {}

  /* eval :: EXPR -> Maybe Int */
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

/*
SUB Sub(EXPR &x, EXPR &y) {
  return SUB(x,y);
}
*/

class DIV : public EXPR
{
  EXPR &x,&y;
  public:
  DIV(EXPR &x_, EXPR &y_): x(x_), y(y_) {}

  /* eval :: EXPR -> Maybe Int */
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

/*
DIV Div(EXPR &x, EXPR &y) {
  return DIV(x,y);
}
*/

class MULT : public EXPR
{
  EXPR &x,&y;
  public:
  MULT(EXPR &x_, EXPR &y_): x(x_), y(y_) {}
  /* eval :: EXPR -> Maybe Int */
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

/*
MULT Mult(EXPR &x, EXPR &y) {
  return MULT(x,y);
}
*/

class NEG: public EXPR
{
  EXPR &x;
  public:
  NEG(EXPR &x_): x(x_) {}

  /* eval :: EXPR -> Maybe Int */
  virtual Maybe<int> eval()
  {
    auto n = VALUE(-1);
    return MULT(n, x).eval();
  }
};

/*
NEG Neg(EXPR &x) {
  return NEG(x);
}
*/

#endif
