//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

constexpr no_name assisted{};

template <class T>
class assisted_injection_impl {
  template <class>
  struct is_assisted : std::false_type {};

  template <class TArg>
  struct is_assisted<named<no_name, TArg>> : std::true_type {};

 public:
  template <class TInjector>
  auto operator()(const TInjector& injector) const {
    return [&](auto&&... args) {
      using ctor = typename type_traits::ctor_traits__<T>::type;
      return this->create(injector, typename ctor::second{}, std::forward<decltype(args)>(args)...);
    };
  }

 private:
  template <class TInitialization, class... TCtor, class TInjector, class... TArgs>
  auto create(const TInjector& injector, const aux::pair<TInitialization, aux::type_list<TCtor...>>& ctor,
              TArgs&&... args) const {
    using seq = std::make_index_sequence<sizeof...(TCtor)>;
    return create_helper(seq{}, injector, ctor, std::forward<decltype(args)>(args)...);
  }

  template <class TInitialization, class... TCtor, std::size_t... Ns, class TInjector, class... TArgs>
  auto create_helper(const std::index_sequence<Ns...>&, const TInjector& injector,
                     const aux::pair<TInitialization, aux::type_list<TCtor...>>&, TArgs&&... args) const {
    return std::make_unique<T>(create_impl<Ns>(injector, aux::type<TCtor>{}, std::forward<TArgs>(args)...)...);
  }

  template <std::size_t, class TInjector, class TArg, class... TArgs, std::enable_if_t<!is_assisted<TArg>::value, int> = 0>
  decltype(auto) create_impl(const TInjector& injector, const aux::type<TArg>&, TArgs&&...) const {
    return injector.template create<TArg>();
  }

  template <std::size_t N, class TInjector, class TArg, class... TArgs>
  decltype(auto) create_impl(const TInjector&, const aux::type<named<no_name, TArg>>&, TArgs&&... args) const {
    constexpr auto value = get_assisted_ctor_nr(N, typename type_traits::ctor_traits__<T>::type::second{});
    return get<TArg>(std::integral_constant<std::size_t, value>{}, args...);
  }

  template <class TInitialization, class... TCtor>
  static constexpr auto get_assisted_ctor_nr(std::size_t N, const aux::pair<TInitialization, aux::type_list<TCtor...>>&) {
    return sum(0, N, is_assisted<TCtor>::value...);
  }

  template <class... Args>
  static constexpr auto sum(std::size_t index, std::size_t N, bool I, Args... V) {
    return index < N ? static_cast<int>(I) + sum(index + 1, N, V...) : static_cast<int>(I);
  }

  static constexpr auto sum(std::size_t, std::size_t) { return 0; }

  template <class TCtor, std::size_t N, class TArg, class... TArgs, class = std::enable_if_t<(N > 1)>>
  decltype(auto) get(const std::integral_constant<std::size_t, N>&, TArg&&, TArgs&&... args) const {
    return get<TCtor>(std::integral_constant<std::size_t, N - 1>{}, args...);
  }

  template <class TCtor, class TArg, class... TArgs>
  decltype(auto) get(const std::integral_constant<std::size_t, 1>&, TArg&& arg, TArgs&&...) const {
    return arg;
  }

  template <class TCtor>
  auto get(const std::integral_constant<std::size_t, 0>&) const {
    return TCtor{};
  }
};

template <class T>
struct assisted_injection : assisted_injection_impl<T> {};

}  // namespace extension
BOOST_DI_NAMESPACE_END
