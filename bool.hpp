#ifndef BOOL_HPP
#define BOOL_HPP

/*
 * Bool interface taken from https://hackage.haskell.org/package/base-4.10.0.0/docs/Data-Bool.html#t:Bool
 */
#include<string>
class Bool {
  const bool b;
  // Operations
  friend Bool operator && (const Bool & x, const Bool & y);
  friend Bool operator || (const Bool & x, const Bool & y);
  friend Bool operator ! (const Bool & x);
  // Member of Eq
  friend Bool operator == (const Bool & x, const Bool & y);
  friend Bool operator != (const Bool & x, const Bool & y);
  // Member of ShowS
  friend Bool operator == (const Bool & x, const Bool & y);
  friend std::string show(const Bool & x);
  public:
  // Allow usage with bool context expressions.(i.e. if statements)
  Bool (const bool & b_) : b(b_) {}
  operator bool() const {return b;}
};

/* data constructors */
const Bool True() { return Bool(true); }
const Bool False() { return Bool(false); }

/* Operations */
Bool operator && (const Bool & x, const Bool & y) { return Bool(x.b && y.b); }
Bool operator || (const Bool & x, const Bool & y) { return Bool(x.b || y.b); }
Bool operator ! (const Bool & x) { return Bool(!x.b); }
const Bool otherwise = True();

template<class A>
A bool_(A f, A t, Bool b) { if (b) { return t;} else {return f;} }

/* Functions for Eq */
Bool operator == (const Bool & x, const Bool & y) { return x.b == y.b; }
Bool operator != (const Bool & x, const Bool & y) { return x.b != y.b; }

/* Functions for ShowS */
/* showsPrec not needed */
std::string show (const Bool & x) {
  static auto t_str = std::string("True");
  static auto f_str = std::string("False");
  if (x.b) { return t_str; } else { return f_str; }
}

#endif //BOOL_HPP
