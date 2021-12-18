#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>

class Valid {
  virtual void _() = 0;
};

class True : public Valid { void _() {} };
class False : public Valid { void _() {} };

template <typename L, typename R>
class And : public False { void _() {} };
template<> class And<True, True> : public True { void _(); };

template <typename L, typename R>
class Or : public True { void _() {} };
template<> class Or<False, False> : public False { void _(); };

void is_true(const True &) {}
void is_false(const False &) {}

True evaluate(const True &);
False evaluate(const False &);

int main() {
  And<True, True> f;
  And<False, True> g;
  And<False, decltype(evaluate(And<True, True>()))> h;
  Or<True, False> i;
  Or<False, False> j;

  is_true(f);
  is_false(g);
  is_false(h);
  is_true(i);
  is_false(j);
}