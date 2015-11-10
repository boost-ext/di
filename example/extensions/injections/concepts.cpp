//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[concepts
//<-
#include <cassert>
#include <type_traits>
//->
#include <boost/di.hpp>

namespace di = boost::di;

#if !defined(__cpp_concepts)
int main() {}
#else

namespace boost {
namespace di {
namespace v1 {
namespace detail {
template <class T>
auto ctor__(int) -> aux::function_traits_t<decltype(&T::template ctor<di::_>)>;

template <class T>
auto ctor__(int) -> aux::function_traits_t<decltype(&T::template ctor<di::_, di::_>)>;

template <class T>
auto ctor__(int) -> aux::function_traits_t<decltype(&T::template ctor<di::_, di::_, di::_>)>;
}
}
}
}

template <class T, class>
struct generic_traits {
  using type = T;
};

template <class T>
struct generic_traits<di::_, T> {
  using type = T;
};

template <class T>
struct generic_traits<di::_&, T> {
  using type = T&;
};

template <class T>
struct generic_traits<const di::_&, T> {
  using type = const T&;
};

template <class T>
struct generic_traits<di::_&&, T> {
  using type = T&&;
};

template <class T>
struct generic_traits<di::_*, T> {
  using type = T*;
};

template <class T>
struct generic_traits<const di::_*, T> {
  using type = const T*;
};

template <class T, template <class...> class TDeleter>
struct generic_traits<std::unique_ptr<di::_, TDeleter<di::_>>, T> {
  using type = std::unique_ptr<T, TDeleter<T>>;
};

template <class T, template <class...> class TDeleter>
struct generic_traits<const std::unique_ptr<di::_, TDeleter<di::_>>&, T> {
  using type = const std::unique_ptr<T, TDeleter<T>>&;
};

template <class T>
struct generic_traits<std::shared_ptr<di::_>, T> {
  using type = std::shared_ptr<T>;
};

template <class T>
struct generic_traits<const std::shared_ptr<di::_>&, T> {
  using type = std::shared_ptr<T>;
};

template <class T>
struct generic_traits<boost::shared_ptr<di::_>, T> {
  using type = boost::shared_ptr<T>;
};

template <class T>
struct generic_traits<const boost::shared_ptr<di::_>&, T> {
  using type = boost::shared_ptr<T>;
};

template <class T>
struct generic_traits<std::weak_ptr<di::_>, T> {
  using type = std::weak_ptr<T>;
};

template <class T>
struct generic_traits<const std::weak_ptr<di::_>&, T> {
  using type = std::weak_ptr<T>;
};

template <class T, class U>
using generic_traits_t = typename generic_traits<T, U>::type;

template <template <class...> class TWrapper, class T, class... Ts, template <class> class X, class Y>
auto generic_cast(TWrapper<T, X<Y>> arg) {
  return static_cast<generic_traits_t<T, di::aux::remove_qualifiers_t<Y>>>(arg.wrapper_);
}

template <class T>
decltype(auto) generic_cast(T&& arg) {
  return arg;
}

struct generics_provider {
  template <class TInitialization, class TMemory, class T, class... TArgs>
  struct is_creatable {
    static constexpr auto value = di::concepts::creatable<TInitialization, T, TArgs...>::value;
  };

  template <class, class T, class TMemory, class... TArgs>
  auto get(const di::type_traits::direct&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T(generic_cast(args)...);
  }

  template <class, class T, class TMemory, class... TArgs>
  auto get(const di::type_traits::uniform&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T{generic_cast(args)...};
  }
};

/*<override `di` provider configuration>*/
class generics_provider_config : public di::config {
 public:
  static auto provider(...) noexcept { return generics_provider{}; }
};

template <typename T>
concept bool Dummy() {
  return requires(T a) {
    { a.dummy() };
  };
}

struct DummyImpl {
  void dummy() {}
};

auto dummy_concept = [] {};

struct example {
  BOOST_DI_INJECT(example, int i, auto t, (named = dummy_concept) auto d) {
    assert(42 == i);
    static_assert(std::is_same<decltype(t), int>::value, "");
    assert(87 == t);
    static_assert(std::is_same<decltype(d), DummyImpl>::value, "");
  }
};

int main() {
  auto injector = di::make_injector<generics_provider_config>(di::bind<int>.to(42), di::bind<di::_>.to(87),
                                                              di::bind<di::_>().named(dummy_concept).to<DummyImpl>());

  injector.create<example>();
}

#endif

//]
