#ifndef MAYBE_HPP
#define MAYBE_HPP

#include "monad.hpp"

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

/* Nothing */
template<class T>
MAYBE<T> Nothing()
{
  MAYBE<T> m;
  m.isNothing = true;
  return m;
}

/* Just a */
template<class A>
MAYBE<A> Just(A a)
{
  MAYBE<A> m;
  m.isNothing = false;
  m.just = a;
  return m;
}

/* isNothing :: Maybe a -> Bool */
template<class A>
bool isNothing(MAYBE<A> m)
{
  return m.isNothing;
}

/* isJust :: Maybe a -> Bool */
template<class A>
bool isJust(MAYBE<A> m)
{
  return ! isNothing(m);
}

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
MAYBE<A> monadReturn(A j)
{
  return Just(j);
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

#endif //MAYBE_HPP
