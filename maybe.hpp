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

  // Constructors
  template<typename A>
  friend const Maybe<A> Nothing();

  template<typename A>
  friend const Maybe<A> Just(A);

  // Operations
  template<class A>
  friend bool isNothing(Maybe<A> m);

  template<class A>
  friend A fromJust(Maybe<A> m);

  // C++ Constructors
  public:
  Maybe() : isNothing(true), just() {};
  Maybe(const T just_) : isNothing(false), just(just_) {};

};

/* Nothing */
template<class T>
const Maybe<T> Nothing()
{
  const Maybe<T> m;
  return m;
}

/* Just a */
template<class A>
const Maybe<A> Just(const A a)
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

/* (>>=) :: (Monad m) => m a -> (a -> m b) -> m b */
/* (>>=) :: Maybe a -> (a -> Maybe b) -> Maybe b */
template<class A, class B>
const Maybe<B> monadBind(Maybe<A> m, std::function<Maybe<B>(A)> f)
{
  if (isJust(m)) {
    return f(fromJust(m));
  } else {
    return Nothing<B>();
  }
}

/* return :: A -> Maybe A */
template<class A>
const Maybe<A> monadReturn(A j)
{
  return Just(j);
}

/* mplus :: MonadPlus m => m a -> m a -> m a */
template<class A>
const Maybe<A> monadPlus(Maybe<A> a, Maybe<A> b) {
  if (isJust(a)) {
    return a;
  } else {
    return b;
  }
}

/* liftM2 :: Monad m => (a1 -> a2 -> r) -> m a1 -> m a2 -> m r */
/* liftM2 :: (a1 -> a2 -> r) -> Maybe a1 -> Maybe a2 -> Maybe r */
template<class A1, class A2, class R>
const Maybe<R> liftM2(std::function<R (A1,A2)> f, Maybe<A1> mx, Maybe<A2> my) {
  return monadBind(mx, std::function<Maybe<R>(A1)> ([&my, f](A1 x){
        return monadBind(my, std::function<Maybe<R>(A2)> ([x, f](A2 y){
              return Just(f(x,y)); })); }));
}

/* join :: Monad m => m (m a) -> m a */
/* join :: Maybe (Maybe a) -> Maybe a */
template<class A>
const Maybe<A> join(Maybe< Maybe<A> > mma) {
  if (isJust(mma)) {
    auto ma = fromJust(mma);
    return ma;
  } else {
    return Nothing<A>();
  }
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
const Maybe<A> listToMaybe(std::vector<A> l)
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
