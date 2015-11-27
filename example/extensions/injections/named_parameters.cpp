//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[named_parameters
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
#if defined(__MSVC__)
int main() {}
#else
#if defined(__CLANG__)
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"

#if (__clang_major__ == 3) && (__clang_minor__ > 4)
#pragma clang diagnostic ignored "-Wunknown-attributes"
#else
#pragma clang diagnostic ignored "-Wattributes"
#endif
#elif defined(__GCC__)
#pragma GCC diagnostic ignored "-Wattributes"
#endif

struct interface {
  virtual ~interface() = default;
  virtual void dummy() = 0;
};
struct implementation : interface {
  void dummy() override {}
};

template <char...>
struct chars {};
struct pair {
  long begin;
  long end;
};

template <class T, T... Chars>
constexpr auto operator""_s() {
  return di::aux::integral_constant<long, const_hash(chars<Chars...>{}, sizeof...(Chars) + 1)>{};
}

long constexpr const_hash(char const* input, long m = 0, long i = 0) {
  return *input && i < m ? static_cast<long>(*input) + 33 * const_hash(input + 1, m, i + 1) : 5381;
}

template <char C, char... Chars>
long constexpr const_hash(const chars<C, Chars...>&, long m = 0, long i = 0) {
  return C && i < m ? static_cast<long>(C) + 33 * const_hash(chars<Chars...>{}, m, i + 1) : 5381;
}

long constexpr const_hash(const chars<>&, ...) { return 5381; }

constexpr pair get_name_impl(const char* input, int begin, int n = 0, int quote = 0) {
  return !*input || *input == ','
             ? pair{0, 0}
             : (quote == 2 ? pair{begin + 1, n} : (quote == 1 && *input == '"'
                                                       ? get_name_impl(input + 1, begin, n, quote + 1)
                                                       : quote == 0 && *input == '"'
                                                             ? get_name_impl(input + 1, begin, n, quote + 1)
                                                             : get_name_impl(input + 1, quote == 0 ? begin + 1 : begin,
                                                                             quote == 1 ? n + 1 : n, quote)));
}

constexpr pair get_name(const char* input, int N, int Q = 0, int i = 0) {
  return Q == N ? get_name_impl(input + 1, i + 1)
                : (*input ? (*input == ',' ? get_name(input + 1, N, Q + 1, i + 1) : get_name(input + 1, N, Q, i + 1))
                          : pair{0, 0});
}

constexpr bool has_names(const char* input) { return *input ? *input == '"' ? true : has_names(input + 1) : false; }

template <class T, class TArg, int N>
struct parse {
  static constexpr auto name = get_name(T::str, N - 1);
  using type = di::aux::conditional_t<
      name.begin == name.end, TArg,
      di::named<di::aux::integral_constant<long, const_hash(&T::str[name.begin], name.end)>, TArg>>;
};

template <class, class, class...>
struct args_impl;

template <class T, class... TArgs, int... Ns>
struct args_impl<T, di::aux::index_sequence<Ns...>, TArgs...> {
  using type = di::aux::type_list<typename parse<T, TArgs, Ns>::type...>;
};

template <class>
struct args;

template <bool, class R, class... TArgs>
struct args__ {
  using type = typename args_impl<R, di::aux::make_index_sequence<sizeof...(TArgs)>, TArgs...>::type;
};

template <class R, class... TArgs>
struct args__<false, R, TArgs...> {
  using type = di::aux::type_list<TArgs...>;
};

template <class R, class... TArgs>
struct args<R (*)(TArgs...)> {
  using type = typename args__<has_names(R::str), R, TArgs...>::type;
};

template <class R>
struct args<R (*)()> {
  using type = di::aux::type_list<>;
};

template <class T>
using args_t = typename args<T>::type;

template <class T>
decltype(T()) ctor_impl1__(...);

template <class T>
auto ctor_impl1__(int) -> decltype(&T::template ctor<di::_>);

template <class T>
decltype(ctor_impl1__<T>(0)) ctor__(...);

template <class T>
auto ctor__(int) -> decltype(&T::ctor);

#define $inject(T, ...)                                          \
  struct boost_di_inject__ {                                     \
    static constexpr auto str = #__VA_ARGS__;                    \
    static boost_di_inject__ ctor(__VA_ARGS__);                  \
    using type = args_t<decltype(ctor__<boost_di_inject__>(0))>; \
  };                                                             \
  T(__VA_ARGS__)

//->

struct example {
  /*<<inject constructor using automatic named parameters>>*/
  $inject(example, int i, std::unique_ptr<interface> up, [[named("my_value")]] int value) {
    assert(i == 42);
    assert(dynamic_cast<implementation*>(up.get()));
    assert(value == 87);
  }
};

int main() {
  auto injector = di::make_injector(
      /*<<bind named parameters>>*/
      di::bind<int>.to(42), di::bind<interface>.to<implementation>(), di::bind<int>.named("my_value"_s).to(87));

  injector.create<example>();
}

//]

#endif
