//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>
#include <type_traits>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

template <class T>
auto ctor__(int) -> aux::function_traits_t<decltype(&T::template ctor<_>)>;

template <class T>
auto ctor__(int) -> aux::function_traits_t<decltype(&T::template ctor<_, _>)>;

template <class T>
auto ctor__(int) -> aux::function_traits_t<decltype(&T::template ctor<_, _, _>)>;

template <class T>
auto ctor__(int) -> aux::function_traits_t<decltype(&T::ctor)>;

template <class T>
aux::type_list<> ctor__(...);

#undef BOOST_DI_INJECT_TRAITS_IMPL_1
#define BOOST_DI_INJECT_TRAITS_IMPL_1(...)                                                       \
  static void ctor(BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_CTOR, __VA_ARGS__)); \
  static void name(BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_NAME, __VA_ARGS__)); \
  using type BOOST_DI_UNUSED =                                                                   \
      detail::combine_t<decltype(ctor__<boost_di_inject__>(0)), aux::function_traits_t<decltype(name)>>;

template <class T, class>
struct concept_traits {
  using type = T;
};

template <class T>
struct concept_traits<_, T> {
  using type = T;
};

template <class T>
struct concept_traits<_&, T> {
  using type = T&;
};

template <class T>
struct concept_traits<const _&, T> {
  using type = const T&;
};

template <class T>
struct concept_traits<_&&, T> {
  using type = T&&;
};

template <class T>
struct concept_traits<_*, T> {
  using type = T*;
};

template <class T>
struct concept_traits<const _*, T> {
  using type = const T*;
};

template <class T, template <class...> class TDeleter>
struct concept_traits<std::unique_ptr<_, TDeleter<_>>, T> {
  using type = std::unique_ptr<T, TDeleter<T>>;
};

template <class T, template <class...> class TDeleter>
struct concept_traits<const std::unique_ptr<_, TDeleter<_>>&, T> {
  using type = const std::unique_ptr<T, TDeleter<T>>&;
};

template <class T>
struct concept_traits<std::shared_ptr<_>, T> {
  using type = std::shared_ptr<T>;
};

template <class T>
struct concept_traits<const std::shared_ptr<_>&, T> {
  using type = std::shared_ptr<T>;
};

template <class T>
struct concept_traits<boost::shared_ptr<_>, T> {
  using type = boost::shared_ptr<T>;
};

template <class T>
struct concept_traits<const boost::shared_ptr<_>&, T> {
  using type = boost::shared_ptr<T>;
};

template <class T>
struct concept_traits<std::weak_ptr<_>, T> {
  using type = std::weak_ptr<T>;
};

template <class T>
struct concept_traits<const std::weak_ptr<_>&, T> {
  using type = std::weak_ptr<T>;
};

template <class T, class U>
using concept_traits_t = typename concept_traits<T, U>::type;

template <template <class...> class TWrapper, class T, class... Ts, template <class...> class X, class TScope, class Y>
auto concept_cast(const TWrapper<T, X<TScope, Y>>& arg) {
  return arg.wrapper_.operator concept_traits_t<T, aux::remove_qualifiers_t<Y>>();
}

template <class T>
decltype(auto) concept_cast(const T& arg) {
  return arg;
}

struct concepts_provider {
  template <class TInitialization, class T, class... TArgs>
  struct is_creatable {
    static constexpr auto value = true;
  };

  template <class T, class TMemory, class... TArgs>
  auto get(const type_traits::direct&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T(concept_cast(args)...);
  }

  template <class T, class TMemory, class... TArgs>
  auto get(const type_traits::uniform&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T{concept_cast(args)...};
  }
};

/*<override `di` provider configuration>*/
class concepts_provider_config : public config {
 public:
  static auto provider(...) noexcept { return concepts_provider{}; }
};

}  // namespace extension
BOOST_DI_NAMESPACE_END
