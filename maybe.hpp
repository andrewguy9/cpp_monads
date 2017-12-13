#ifndef MAYBE_HPP
#define MAYBE_HPP

#include "monad.hpp"

/*
 * define maybe typeclass
 */
template <class T>
class Maybe : public MONAD<T> {
  const bool isNothing;
  const T just;

  Maybe() : isNothing(true), just() {}
  Maybe(const T just_) : isNothing(false), just(just_) {};

  template<typename A>
  friend Maybe<A> Nothing();

  template<typename A>
  friend Maybe<A> Just(A);

  template<class A>
  friend bool isNothing(Maybe<A> m);

  template<class A>
  friend A fromJust(Maybe<A> m);
};

/* Nothing */
template<class T>
Maybe<T> Nothing()
{
  Maybe<T> m;
  return m;
}

/* Just a */
template<class A>
Maybe<A> Just(A a)
{
  Maybe<A> m(a);
  return m;
}

/* isNothing :: Maybe a -> Bool */
template<class A>
bool isNothing(Maybe<A> m)
{
  return m.isNothing;
}

/* isJust :: Maybe a -> Bool */
template<class A>
bool isJust(Maybe<A> m)
{
  return ! isNothing(m);
}

/* (>>=) :: Maybe a -> (a -> Maybe b) -> Maybe b */
template<class A, class B>
Maybe<B> monadBind(Maybe<A> m, std::function<Maybe<B>(A)> f)
{
  if (isJust(m)) {
    return f(fromJust(m));
  } else {
    return Nothing<B>();
  }
}

/* return :: A -> Maybe A */
template<class A>
Maybe<A> monadReturn_(A j)
{
  return Just(j);
}

/* maybe :: b -> (a -> b) -> Maybe a -> b */
template<class A, class B>
B maybe(B default_, B foo(A), Maybe<A> m)
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
A fromJust(Maybe<A> m)
{
  if (isJust(m)) {
    return m.just;
  } else {
    throw std::invalid_argument("fromJust called on Nothing");
  }
}

/* fromMaybe :: a -> Maybe a -> a */
template<class A>
A fromMaybe(A default_, Maybe<A> m)
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
Maybe<A> listToMaybe(std::vector<A> l)
{
  for (auto x: l) {
    return Just(x);
  }
  return Nothing<A>();
}

/* maybeToList :: Maybe a -> [a] */
// Doesn't map to c++.
template<class A>
std::vector<A> maybeToList(Maybe<A> m)
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
std::vector<A> catMaybes(std::vector<Maybe<A> > l)
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
std::vector<B> mapMaybe(Maybe<B> foo(A), std::vector<A> l)
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
bool operator ==(const Maybe<A> &a, const Maybe<A> &b)
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
bool operator <(const Maybe<A> &a, const Maybe<A> &b)
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
