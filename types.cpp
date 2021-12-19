#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>

template <typename R>
class Result {
  public:
  using result = R;
  virtual void _() = 0;
};

template <typename R>
class Boolean : public Result<R> { };

class True : public Boolean<True> { void _() {} };
class False : public Boolean<False> { void _() {} };

template <typename L, typename R>
class And : public And<typename L::result, typename R::result> { void _() {} };
template<> class And<True, True> : public Boolean<True> { void _() {} };
template<> class And<True, False> : public Boolean<False> { void _() {} };
template<> class And<False, True> : public Boolean<False> { void _() {} };
template<> class And<False, False> : public Boolean<False> { void _() {} };

template <typename L, typename R>
class Or : public Or<typename L::result, typename R::result> { void _() {} };
template<> class Or<True, True> : public Boolean<True> { void _() {} };
template<> class Or<True, False> : public Boolean<True> { void _() {} };
template<> class Or<False, True> : public Boolean<True> { void _() {} };
template<> class Or<False, False> : public Boolean<False> { void _() {} };

template <typename L>
class Not : public Not<typename L::result> { void _() {} };
template<> class Not<False> : public Boolean<True> { void _() {} };
template<> class Not<True> : public Boolean<False> { void _() {} };


template <typename C, typename T, typename F>
class If : public If<typename C::result, T, F> { void _() {} };

template<typename L, typename R>
class If<True, L, R> : public L::result { void _() {}};

template<typename L, typename R>
class If<False, L, R> : public R::result { void _() {}};

template <typename R>
class Integer : public Result<R> { void _() {} };

class Zero : public Result<Zero> { void _() {} };

template <typename I>
class Succ;
template<> class Succ<Zero> : public Result<Succ<Zero>> { void _() {} };
template<typename I> class Succ<Succ<I>> : public Result<Succ<Succ<typename I::result>>> { void _() {}};

template <typename L, typename R>
class Add;
template<typename L> class Add<Succ<L>, Zero> : public Succ<L> { void _() {} };
template<typename R> class Add<Zero, Succ<R>> : public Succ<R> { void _() {} };
template<typename L, typename R> class Add<L, Succ<R>> : public Succ<typename Add<typename L::result, R>::result> { void _() {} };

void is_true(const True &) {}
void is_false(const False &) {}

template <typename T>
void print_result(const T &result) {
  std::cout << typeid(result).name() << "\n";
}

template <typename T>
constexpr int succ_to_int(const Succ<T> &v) {
  return 1 + succ_to_int(T());
}
constexpr int succ_to_int(const Zero &z) {
  return 0;
}

int main() {
  using compile = If<And<False, True>, Not<Not<False>>, True>::result;
  //print_result(compile());

  using one = Succ<Zero>;
  using two = Add<one, one>;
  using three = Succ<Succ<Succ<Zero>>>;
  using four = Succ<three>;
  using five = Add<four, one>;
  using six = Add<five, one>;

/*
  print_result(one::result());
  print_result(two::result());
  print_result(two_add::result());
  print_result(three::result());
  print_result(four::result());
  print_result(five::result());
  print_result(six::result());
*/

  static_assert(1 == succ_to_int(one::result()), "1 != 1");
  static_assert(2 == succ_to_int(two::result()), "2 != 2");
  static_assert(3 == succ_to_int(three::result()), "3 != 3");
  static_assert(4 == succ_to_int(four::result()), "4 != 4");
  static_assert(5 == succ_to_int(five::result()), "5 != 5");
  static_assert(6 == succ_to_int(six::result()), "5 != 5");

/*
  std::cout << "1 == " << succ_to_int(one::result()) << "\n";
  std::cout << "2 == " << succ_to_int(two::result()) << "\n";
  std::cout << "2 == " << succ_to_int(two_add::result()) << "\n";
  std::cout << "3 == " << succ_to_int(three::result()) << "\n";
  std::cout << "4 == " << succ_to_int(four::result()) << "\n";
  std::cout << "5 == " << succ_to_int(five::result()) << "\n";
  std::cout << "6 == " << succ_to_int(six::result()) << "\n";
*/
  return 0;
}