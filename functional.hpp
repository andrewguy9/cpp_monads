#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

/* curry :: ((a, b) -> c) -> a -> b -> c */
//May not be useful in C++ where we dont' have curring anyway.

/* uncurry :: (a -> b -> c) -> (a, b) -> c */
#include<functional>
#include<utility>
template <class A, class B, class C>
std::function<C (std::pair<A,B>)> uncurry(std::function<C (A,B)> f) {
    return [f](std::pair<A,B> i) {
      return f(i.first, i.second);
    };
}

#endif /* FUNCTIONAL_HPP */
