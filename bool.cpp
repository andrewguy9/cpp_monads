#include<string>
class Bool {
  const bool b;
  Bool (const bool & b_) : b(b_) {}
  friend const Bool True();
  friend const Bool False();
  friend Bool operator == (const Bool & x, const Bool & y);
  friend std::string show(const Bool & x);
  public:
};

Bool operator == (const Bool & x, const Bool & y) { return x.b == y.b; }

std::string show (const Bool & x) {
  static auto t_str = std::string("True");
  static auto f_str = std::string("False");

  if (x.b) { return t_str; }
  else { return f_str; }
}

const Bool True() { return Bool(true); }
const Bool False() { return Bool(false); }

using namespace std;
#include<iostream>
int main() {
  auto x = True();
  auto y = True();

  cout << show(x) << endl;
  cout << show(True() == False()) << endl;
  cout << show(x == y) << endl;
  /* cout << True() == False() << endl; */ //Error: Bool isn't printable, use show().
}
