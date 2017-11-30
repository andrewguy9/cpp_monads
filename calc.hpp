#ifndef CALC_H
#define CALC_H

#include "maybe.hpp"

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


#endif
