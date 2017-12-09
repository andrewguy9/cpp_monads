#ifndef MONAD_HPP
#define MONAD_HPP

/*
 * Monad Tools:
 * define monad typeclass
 */


#include<functional>
template <class T>
class MONAD {
  public:
};

#if 0
/* bind:: Monad m => m A -> (A -> m B) -> m B */
template<class A, class B>
MONAD<B> monadBind(MONAD<A> m, std::function<MONAD<B>(A)> f);
#endif

#if 0
/* return :: Monad m => A -> m A */
template<class A>
MONAD<A> monadReturn(A j);
#endif

#endif //MONAD_HPP
