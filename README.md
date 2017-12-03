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

C++:

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

Haskell:
``` haskell
x = True;
y = False;
```

C++:
``` c++
auto x = True();
auto y = False();
```

Operations:

Haskell:
``` haskell
(&&) :: Bool -> Bool -> Bool
(||) :: Bool -> Bool -> Bool
not :: Bool -> Bool
```
C++
``` c++
class Bool {
  const bool b;
  ...
  // Operations
  friend Bool operator && (const Bool & x, const Bool & y);
  friend Bool operator || (const Bool & x, const Bool & y);
  friend Bool operator ! (const Bool & x);
  ...
};

Bool operator && (const Bool & x, const Bool & y) { return Bool(x.b && y.b); }
Bool operator || (const Bool & x, const Bool & y) { return Bool(x.b || y.b); }
Bool operator ! (const Bool & x) { return Bool(!x.b); }
const Bool otherwise = True();
```

