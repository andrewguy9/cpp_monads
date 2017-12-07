#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include<functional>  // for std::function
#include<utility> // for std::pair

/* curry :: ((a, b) -> c) -> a -> b -> c */
// May not be useful in C++ where we dont' have curring anyway.
template <class A, class B, class C>
std::function< std::function<C (B)> (A)>
curry(std::function<C (std::pair<A,B>)> f) {
  return [f] (A a) { // std::function< std::function<C (B)> (A)>
    return [f,a] (B b) { // std::function<C (B)>
      return f(std::pair<A,B>(a,b)); // C
    };
  };
}

/* uncurry :: (a -> b -> c) -> (a, b) -> c */
template <class A, class B, class C>
std::function<C (std::pair<A,B>)>
uncurry(std::function<C (A,B)> f) {
    return [f](std::pair<A,B> i) {
      return f(i.first, i.second);
    };
}

#endif /* FUNCTIONAL_HPP */
