//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <utility>
#include <type_traits>

template<class TParent>
struct any_type {
  template<class T, class = std::enable_if_t<!std::is_convertible<TParent, T>{}>>
  operator T();
};

constexpr auto BOOST_DI_CFG_CTOR_LIMIT_SIZE = 10;

template<class T, std::size_t>
using any_type_t = any_type<T>;

template<class...>
struct is_constructible;

template<class T, std::size_t... Ns>
struct is_constructible<T, std::index_sequence<Ns...>>
  : std::is_constructible<T, any_type_t<T, Ns>...>
{ };

template <class T, std::size_t... Ns>
constexpr auto ctor(std::index_sequence<Ns...>) noexcept {
  auto value = 0;
  int _[]{0, (is_constructible<T, std::make_index_sequence<Ns>>{} ? value = Ns : value)...};
  (void)_;
  return value;
}

int main() {
  struct example {
    example(int, double, float);
  };

  static_assert(3 == ctor<example>(std::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE>{}));
}
