#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>


template <typename T>
void print_value(const T &value) {
  std::cout << typeid(value).name() << "\n";
}

template <typename R>
class Singleton {
  public:
  using value = R;
  virtual void _() = 0;
};

class None : Singleton<std::nullptr_t> {};

template <typename R>
class Boolean : public Singleton<R> { };

class True : public Boolean<True> { void _() {} };
class False : public Boolean<False> { void _() {} };

template <typename L, typename R>
class And : public And<typename L::value, typename R::value> { void _() {} };
template<> class And<True, True> : public Boolean<True> { void _() {} };
template<> class And<True, False> : public Boolean<False> { void _() {} };
template<> class And<False, True> : public Boolean<False> { void _() {} };
template<> class And<False, False> : public Boolean<False> { void _() {} };

template <typename L, typename R>
class Or : public Or<typename L::value, typename R::value> { void _() {} };
template<> class Or<True, True> : public Boolean<True> { void _() {} };
template<> class Or<True, False> : public Boolean<True> { void _() {} };
template<> class Or<False, True> : public Boolean<True> { void _() {} };
template<> class Or<False, False> : public Boolean<False> { void _() {} };

template <typename L>
class Not : public Not<typename L::value> { void _() {} };
template<> class Not<False> : public Boolean<True> { void _() {} };
template<> class Not<True> : public Boolean<False> { void _() {} };


template <typename C, typename T, typename F>
class If : public If<typename C::value, T, F> { void _() {} };

template<typename L, typename R>
class If<True, L, R> : public L::value { void _() {}};

template<typename L, typename R>
class If<False, L, R> : public R::value { void _() {}};

template <typename R>
class Integer : public Singleton<R> { void _() {} };

class Zero : public Integer<Zero> { void _() {} };

template <typename I>
class Succ;
template<> class Succ<Zero> : public Integer<Succ<Zero>> { void _() {} };
template<typename I> class Succ<Succ<I>> : public Integer<Succ<Succ<typename I::value>>> { void _() {}};

template <typename L, typename R>
class Add : public Add<typename L::value, typename R::value> { void _() {} };
template<typename L> class Add<Succ<L>, Zero> : public Integer<Succ<L>> { void _() {} };
template<typename R> class Add<Zero, Succ<R>> : public Integer<Succ<R>> { void _() {} };
template<typename L, typename R> class Add<L, Succ<R>> : public Integer<Succ<typename Add<typename L::value, R>::value>> { void _() {} };

template <typename L, typename R>
class Equal : public Boolean<False> { void _(); };

template <>
class Equal<True, True> : public Boolean<True> { void _() {} };
template <>
class Equal<False, False> : public Boolean<True> { void _() {} };

template <>
class Equal<False, True> : public Boolean<False> { void _() {} };
template <>
class Equal<True, False> : public Boolean<False> { void _() {} };
template <typename R>
class Equal<True, R> : public Equal<True, typename R::value> { void _() {} };

template<>
class Equal<Zero, Zero> : public Boolean<True> { void _() {} };

template <typename L, typename R>
class Equal<Integer<L>, Integer<R>> : public Equal<L, R>::value { void _() {} };

template <typename L, typename R>
class Equal<Succ<L>, Succ<R>> : public Equal<L, R>::value { void _() {} };

template <typename L>
class Monad : public Singleton<Monad<L>> { void _() {} };

template <typename L>
class Printable : public Monad<L> { public: Printable() { std::cout << "This is a printable side effect!\n"; }};


// Lists.

template <typename Head, typename Tail>
class List;
template <typename Head>
class List<Head, None> {
  public:
  using value = List<Head, None>;
  using head = typename Head::value;
  using tail = None;
  void _() {}
};

template <typename Head, typename TailHead, typename Tail>
class List<Head, List<TailHead, Tail>> {
  public:
  using value = List<Head, List<TailHead, Tail>>;
  using head = typename Head::value;
  using tail = List<TailHead, Tail>;

  void _() {}
};

template <typename L>
class Head : public L::head { void _() {}};

void is_true(const True &) {}
void is_false(const False &) {}


template <typename T>
constexpr int succ_to_int(const Succ<T> &v) {
  return 1 + succ_to_int(T());
}
constexpr int succ_to_int(const Zero &z) {
  return 0;
}

template <typename Head, typename Tail>
void print_list(const List<Head, Tail> &value) {
  std::cout << typeid(Head()).name() << "\n";
  print_list(Tail());
}

template <typename Head>
void print_list(const List<Head, None> &value) {
  std::cout << typeid(Head()).name() << "\n";
}

int main() {
  using compile = If<And<False, True>, Not<Not<False>>, True>::value;
  //print_value(compile());

  using One = Succ<Zero>;
  using Two = Add<One, One>::value;
  using Three = Succ<Succ<Succ<Zero>>>;
  using Four = Succ<Three>;
  using Five = Add<Four, One>::value;
  using Six = Add<Five, One>::value;
  using Ten = Add<Five, Five>::value;
  //using one_hundred = Add<ten, Add<ten, Add<ten, ten>::value>::value>::value;
  using OneHundred = Add<Ten, Add<Ten, Add<Ten, Add<Ten, Add<Ten, Add<Ten, Add<Ten, Add<Ten, Add<Ten, Ten>>>>>>>>>::value;

/*
  print_value(one::value());
  print_value(two::value());
  print_value(two_add::value());
  print_value(three::value());
  print_value(four::value());
  print_value(five::value());
  print_value(six::value());
*/

  static_assert(1 == succ_to_int(One::value()), "1 != 1");
  static_assert(2 == succ_to_int(Two::value()), "2 != 2");
  static_assert(3 == succ_to_int(Three::value()), "3 != 3");
  static_assert(4 == succ_to_int(Four::value()), "4 != 4");
  static_assert(5 == succ_to_int(Five::value()), "5 != 5");
  static_assert(6 == succ_to_int(Six::value()), "5 != 5");
  static_assert(100 == succ_to_int(OneHundred::value()), "100 != 100");

  is_false(Equal<One, Two>::value());
  is_true(Equal<Two, Two>::value());
  is_false(Equal<Zero, Two>::value());
  is_true(Equal<True, True>::value());
  is_true(Equal<False, False>::value());
  is_false(Equal<True, False>::value());
  is_false(Equal<False, True>::value());
  is_true(Equal<OneHundred, OneHundred>::value());
  is_false(Not<Equal<OneHundred, OneHundred>>::value());
  is_false(Equal<Ten, False>::value());
  is_false(Equal<False, Ten>::value());
  is_false(Equal<False, Zero>::value());

  Printable<Zero> _;

/*
  std::cout << "1 == " << succ_to_int(one::value()) << "\n";
  std::cout << "2 == " << succ_to_int(two::value()) << "\n";
  std::cout << "2 == " << succ_to_int(two_add::value()) << "\n";
  std::cout << "3 == " << succ_to_int(three::value()) << "\n";
  std::cout << "4 == " << succ_to_int(four::value()) << "\n";
  std::cout << "5 == " << succ_to_int(five::value()) << "\n";
  std::cout << "6 == " << succ_to_int(six::value()) << "\n";
*/

  Head<List<One, List<Zero, List<Zero, None>>>> head;
  print_list(List<One, List<Zero, List<Zero, None>>>());

  is_true(Equal<One, Head<List<One, List<Zero, List<Zero, None>>>>::value>::value());
  return 0;
}