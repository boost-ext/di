//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <type_traits>
#include <utility>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

template <class...>
struct valid {
  using type = int;
};

template <class... Ts>
using valid_t = typename valid<Ts...>::type;

template <int, class T>
struct type_id_type {};

template <class, class...>
struct type_id_impl;

template <std::size_t... Ns, class... Ts>
struct type_id_impl<std::index_sequence<Ns...>, Ts...> : type_id_type<Ns, Ts>... {};

template <class... Ts>
struct type_id : type_id_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...> {};

template <class T, int, int N>
auto get_id_impl(type_id_type<N, T>*) {
  return N;
}
template <class T, int D>
auto get_id_impl(...) {
  return D;
}
template <class TIds, int D, class T>
auto get_id() {
  return get_id_impl<T, D>((TIds*)0);
}

template <class T>
struct identity {
  using type = T;
};

template <int N, class D, class T>
auto get_type_impl(type_id_type<N, T>*) {
  return identity<T>{};
}
template <int, class D>
auto get_type_impl(...) {
  return identity<D>{};
}

template <class TIds, class D, int N>
struct get_type {
  using underlying_type = decltype(get_type_impl<N, D>((TIds*)0));
  using type = typename underlying_type::type;
};

template <class TIds, int N, class D = void>
using get_type_t = typename get_type<TIds, D, N>::type;

template <class>
struct get_size;

template <template <class...> class T, class... Ts>
struct get_size<T<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};

template <class>
struct function_traits;

template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...) const> {
  using type = R (*)(const void*, TArgs...);
  using result = R;
  using args = type_id<TArgs...>;
};

template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...)> {
  using type = R (*)(const void*, TArgs...);
  using result = R;
  using args = type_id<TArgs...>;
};

template <class, int, class = int>
struct has_info__ : std::false_type {};

template <class T, int N>
struct has_info__<T, N, valid_t<typename T::template info__<N, void>::type>> : std::true_type {};

#define ARGS_IMPL(i, ...) \
  __BOOST_DI_IF(i, __BOOST_DI_COMMA, __BOOST_DI_EAT)()::boost::di::extension::get_type_t<typename type::args, i> p##i
#define ARGS(...)                                                                    \
  __BOOST_DI_IF(__BOOST_DI_IS_EMPTY(__VA_ARGS__), __BOOST_DI_EAT, __BOOST_DI_REPEAT) \
  (__BOOST_DI_SIZE(__VA_ARGS__), ARGS_IMPL, __VA_ARGS__)

#define PASS_IMPL(i, ...) __BOOST_DI_IF(i, __BOOST_DI_COMMA, __BOOST_DI_EAT)() p##i
#define PASS(...)                                                                    \
  __BOOST_DI_IF(__BOOST_DI_IS_EMPTY(__VA_ARGS__), __BOOST_DI_EAT, __BOOST_DI_REPEAT) \
  (__BOOST_DI_SIZE(__VA_ARGS__), PASS_IMPL, __VA_ARGS__)
#define COMMA_IF(...) __BOOST_DI_IF(__BOOST_DI_IS_EMPTY(__VA_ARGS__), __BOOST_DI_EAT, __BOOST_DI_COMMA)()

#define GENERIC(name)                                                                                              \
  static constexpr auto id = __COUNTER__ + 1;                                                                      \
  using self_t = name;                                                                                             \
  template <int, class = void>                                                                                     \
  struct info__;                                                                                                   \
  template <int N, std::enable_if_t<!::boost::di::extension::has_info__<self_t, id + N>::value, int> = 0>          \
  static constexpr auto count() {                                                                                  \
    return N;                                                                                                      \
  }                                                                                                                \
  template <int N, std::enable_if_t<::boost::di::extension::has_info__<self_t, id + N>::value, int> = 0>           \
  static constexpr auto count() {                                                                                  \
    return count<N + 1>();                                                                                         \
  }                                                                                                                \
  template <class T>                                                                                               \
  static auto& vtable() {                                                                                          \
    static void* fs[count<0>()];                                                                                   \
    return fs;                                                                                                     \
  }                                                                                                                \
  const void* self;                                                                                                \
  void** ptr;                                                                                                      \
  name() = default;                                                                                                \
  template <class T>                                                                                               \
  inline name(const T& t, int) : self(&t), ptr(vtable<std::decay_t<T>>()) {                                        \
    name_impl<std::decay_t<T>, 0>(ptr);                                                                            \
  }                                                                                                                \
  template <class T, int N, std::enable_if_t<!::boost::di::extension::has_info__<self_t, id + N>::value, int> = 0> \
  static void name_impl(void**) {}                                                                                 \
  template <class T, int N, std::enable_if_t<::boost::di::extension::has_info__<self_t, id + N>::value, int> = 0>  \
  static void name_impl(void** ptr) {                                                                              \
    auto f = &info__<id + N>::template f<T>;                                                                       \
    ptr[N] = *(void**)(&f);                                                                                        \
    name_impl<T, N + 1>(ptr);                                                                                      \
  }                                                                                                                \
  template <class T>                                                                                               \
  inline name(const T& t) : name(t, 0)

#define REQUIRES(name, ...) REQUIRES_IMPL(__COUNTER__, name, __VA_ARGS__)
#define REQUIRES_IMPL(i, name, ...)                                                                                     \
  { return info__<i>::call(self, ptr COMMA_IF(__VA_ARGS__) __VA_ARGS__); }                                              \
  template <class V>                                                                                                    \
  struct info__<i, V> {                                                                                                 \
    using type = ::boost::di::extension::function_traits<decltype(&self_t::name)>;                                      \
    template <class T>                                                                                                  \
    static inline typename type::result f(const void* ptr COMMA_IF(__VA_ARGS__) ARGS(__VA_ARGS__)) {                    \
      return ((T*)ptr)->name(PASS(__VA_ARGS__));                                                                        \
    }                                                                                                                   \
    static inline typename type::result call(const void* self, void** vtable COMMA_IF(__VA_ARGS__) ARGS(__VA_ARGS__)) { \
      return (*(type::type*)(&vtable[i - id]))(self COMMA_IF(__VA_ARGS__) PASS(__VA_ARGS__));                           \
    }                                                                                                                   \
  }

template <template <class...> class TWrapper, class T, class... Ts, template <class, class> class X, class TScope, class Y>
auto generic_cast(const TWrapper<T, X<TScope, Y>>& arg) {
  return arg.wrapper_.operator T();
}

template <class T>
decltype(auto) generic_cast(const T& arg) {
  return arg;
}

struct generics_provider {
  template <class TInitialization, class T, class... TArgs>
  struct is_creatable {
    static constexpr auto value = true;
  };

  template <class T, class TMemory, class... TArgs>
  auto get(const di::type_traits::direct&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T(generic_cast(args)...);
  }

  template <class T, class TMemory, class... TArgs>
  auto get(const di::type_traits::uniform&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T{generic_cast(args)...};
  }
};

}  // extension
BOOST_DI_NAMESPACE_END
