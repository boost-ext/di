//
// Copyright (c) 2012-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <memory>
#include <type_traits>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
#if !defined(__cpp_concepts)
int main() {}
#else

template <class T>
auto ctor__(int) -> di::aux::function_traits_t<decltype(&T::template ctor<di::_>)>;

template <class T>
auto ctor__(int) -> di::aux::function_traits_t<decltype(&T::template ctor<di::_, di::_>)>;

template <class T>
auto ctor__(int) -> di::aux::function_traits_t<decltype(&T::template ctor<di::_, di::_, di::_>)>;

template <class T>
auto ctor__(int) -> di::aux::function_traits_t<decltype(&T::ctor)>;

template <class T>
di::aux::type_list<> ctor__(...);

#undef BOOST_DI_INJECT_TRAITS_IMPL_1
#define BOOST_DI_INJECT_TRAITS_IMPL_1(...)                                                       \
  static void ctor(BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_CTOR, __VA_ARGS__)); \
  static void name(BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_NAME, __VA_ARGS__)); \
  using type BOOST_DI_UNUSED =                                                                   \
      di::detail::combine_t<decltype(ctor__<boost_di_inject__>(0)), di::aux::function_traits_t<decltype(name)>>;

template <class T, class>
struct concept_traits {
  using type = T;
};

template <class T>
struct concept_traits<di::_, T> {
  using type = T;
};

template <class T>
struct concept_traits<di::_&, T> {
  using type = T&;
};

template <class T>
struct concept_traits<const di::_&, T> {
  using type = const T&;
};

template <class T>
struct concept_traits<di::_&&, T> {
  using type = T&&;
};

template <class T>
struct concept_traits<di::_*, T> {
  using type = T*;
};

template <class T>
struct concept_traits<const di::_*, T> {
  using type = const T*;
};

template <class T, template <class...> class TDeleter>
struct concept_traits<std::unique_ptr<di::_, TDeleter<di::_>>, T> {
  using type = std::unique_ptr<T, TDeleter<T>>;
};

template <class T, template <class...> class TDeleter>
struct concept_traits<const std::unique_ptr<di::_, TDeleter<di::_>>&, T> {
  using type = const std::unique_ptr<T, TDeleter<T>>&;
};

template <class T>
struct concept_traits<std::shared_ptr<di::_>, T> {
  using type = std::shared_ptr<T>;
};

template <class T>
struct concept_traits<const std::shared_ptr<di::_>&, T> {
  using type = std::shared_ptr<T>;
};

template <class T>
struct concept_traits<boost::shared_ptr<di::_>, T> {
  using type = boost::shared_ptr<T>;
};

template <class T>
struct concept_traits<const boost::shared_ptr<di::_>&, T> {
  using type = boost::shared_ptr<T>;
};

template <class T>
struct concept_traits<std::weak_ptr<di::_>, T> {
  using type = std::weak_ptr<T>;
};

template <class T>
struct concept_traits<const std::weak_ptr<di::_>&, T> {
  using type = std::weak_ptr<T>;
};

template <class T, class U>
using concept_traits_t = typename concept_traits<T, U>::type;

template <template <class...> class TWrapper, class T, class... Ts, template <class...> class X, class TScope, class Y>
auto concept_cast(const TWrapper<T, X<TScope, Y>>& arg) {
  return arg.wrapper_.operator concept_traits_t<T, di::aux::remove_qualifiers_t<Y>>();
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
  auto get(const di::type_traits::direct&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T(concept_cast(args)...);
  }

  template <class T, class TMemory, class... TArgs>
  auto get(const di::type_traits::uniform&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T{concept_cast(args)...};
  }
};

/*<override `di` provider configuration>*/
class concepts_provider_config : public di::config {
 public:
  static auto provider(...) noexcept { return concepts_provider{}; }
};
//->

template <typename T>
concept bool Dummy() {
  return requires(T a) { {a.dummy()}; };
}

struct DummyImpl {
  void dummy() {}
};

auto dummy_concept = [] {};

struct example {
  BOOST_DI_INJECT(example, int i, auto t, (named = dummy_concept) Dummy d, (named = dummy_concept) std::unique_ptr<Dummy> up) {
    assert(42 == i);
    static_assert(std::is_same<decltype(t), int>::value, "");
    assert(87 == t);
    static_assert(std::is_same<decltype(d), DummyImpl>::value, "");
    static_assert(std::is_same<decltype(up), std::unique_ptr<DummyImpl>>::value, "");
  }
};

int main() {
  // clang-format off
  auto injector = di::make_injector<concepts_provider_config>(
    di::bind<int>.to(42)
  , di::bind<di::_>.to(87)
  , di::bind<di::_>().named(dummy_concept).to<DummyImpl>()
  );
  // clang-format on

  injector.create<example>();
}

#endif
