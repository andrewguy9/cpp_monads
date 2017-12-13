#include "calc.hpp"

#include<string>
#include<vector>
#include<map>
#include<utility>
#include<iostream>

using namespace std;

int main(int argc, char ** argv)
{
  vector<Value> stack;
  for (int i = 1; i < argc; i++) {
    string s = argv[i];
    if (0 == s.compare("+")) {
      //TODO would be nice to have an abstraction for application.
      Plus p(stack[0],stack[1]);
      stack.pop_back();
      stack.pop_back();
      Value r = fromJust(p.eval());
      stack.push_back(r);
    } else if (0 == s.compare("-")) {
      //TODO would be nice to have an abstraction for application.
      Sub b(stack[0],stack[1]);
      stack.pop_back();
      stack.pop_back();
      Value r = fromJust(b.eval());
      stack.push_back(r);
    } else if (s == "*") {
      //TODO would be nice to have an abstraction for application.
      Mult m(stack[0],stack[1]);
      stack.pop_back();
      stack.pop_back();
      Value r = fromJust(m.eval());
      stack.push_back(r);
    } else if (s == "/") {
      //TODO would be nice to have an abstraction for application.
      Div d(stack[0],stack[1]);
      stack.pop_back();
      stack.pop_back();
      Value r = fromJust(d.eval());
      stack.push_back(r);
    } else if (s == "_") {
      //TODO would be nice to have an abstraction for application.
      Neg n(stack[0]);
      stack.pop_back();
      Value r = fromJust(n.eval());
      stack.push_back(r);
    } else {
      try {
        Value v(stoi(s));
        stack.push_back(v);
      } catch (invalid_argument) {
        cout << "caught invalid arg:" << s << endl;
        return 1;
      }
    }
  }

  if (stack.size() > 1) {
    cout << "Stack has more than one element" << endl;
    return 2;
  } else if (stack.size() == 0) {
    cout << "Stack is empty!" << endl;
  } else {
    Value first = stack[0];
    cout << fromJust(first.eval()) << endl;
    return 0;
  }
}
