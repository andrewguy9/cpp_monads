Building Monads in C++

This is a guide/example of implementing various Haskell programming techniques in c++.

`data` types
===

Example: Bool
---
Haskell:

``` haskell
data Bool = False | True
```

Cpp:

``` c++
class Bool {
  const bool b;
  Bool (const bool & b_) : b(b_) {}
  friend const Bool True(const bool & b_);
  friend const Bool False(const bool & b_);
};

const Bool True() { return Bool(true); }
const Bool False() { return Bool(false); }
```

Rules:
* Data type names should be `CamelCase`.
* All data members must be `const`.
* Make a non-default constructor which is `private`.
* Make external functions which act as constructors / factories.
    * They must be friends to the class so that they can call the private constructor.

``` haskell
x = True;
y = False;
```

``` c++
auto x = True();
auto y = False();
```
