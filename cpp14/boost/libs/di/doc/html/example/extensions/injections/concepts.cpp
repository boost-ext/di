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
#include <typeinfo>
#include <iostream>

#if !defined(__cpp_concepts)
int main() {}
#else

namespace boost {
namespace di {
namespace v1 {
namespace detail {
template <class T>
auto ctor__(int) -> aux::function_traits_t<decltype(&T::template ctor<_>)>;

template <class T>
auto ctor__(int) -> aux::function_traits_t<decltype(&T::template ctor<_, _>)>;

template <class T>
auto ctor__(int) -> aux::function_traits_t<decltype(&T::template ctor<_, _, _>)>;
}
}
}
}

namespace boost {
namespace di {
inline namespace v1 {
namespace type_traits {
template <class T>
struct _eric_cast {
  using type = T&&;
};
template <template <class...> class T, class... Ts>
struct _eric_cast<T<Ts...>> {
  using type = typename T<Ts...>::value_type;
};
template <class T>
using _eric_cast_t = typename _eric_cast<T>::type;
template <class T, class>
struct _eric_traits {
  using type = T;
};
template <class T>
struct _eric_traits<_, T> {
  using type = T;
};
template <class T>
struct _eric_traits<_&, T> {
  using type = T&;
};
template <class T>
struct _eric_traits<const _&, T> {
  using type = const T&;
};
template <class T>
struct _eric_traits<_&&, T> {
  using type = T&&;
};
template <class T>
struct _eric_traits<_*, T> {
  using type = T*;
};
template <class T>
struct _eric_traits<const _*, T> {
  using type = const T*;
};
template <class T, template <class...> class TDeleter>
struct _eric_traits<std::unique_ptr<_, TDeleter<_>>, T> {
  using type = std::unique_ptr<T, TDeleter<T>>;
};
template <class T, template <class...> class TDeleter>
struct _eric_traits<const std::unique_ptr<_, TDeleter<_>>&, T> {
  using type = const std::unique_ptr<T, TDeleter<T>>&;
};
template <class T>
struct _eric_traits<std::shared_ptr<_>, T> {
  using type = std::shared_ptr<T>;
};
template <class T>
struct _eric_traits<const std::shared_ptr<_>&, T> {
  using type = std::shared_ptr<T>;
};
template <class T>
struct _eric_traits<boost::shared_ptr<_>, T> {
  using type = boost::shared_ptr<T>;
};
template <class T>
struct _eric_traits<const boost::shared_ptr<_>&, T> {
  using type = boost::shared_ptr<T>;
};
template <class T>
struct _eric_traits<std::weak_ptr<_>, T> {
  using type = std::weak_ptr<T>;
};
template <class T>
struct _eric_traits<const std::weak_ptr<_>&, T> {
  using type = std::weak_ptr<T>;
};
template <class T, class U>
using _eric_traits_t = typename _eric_traits<T, U>::type;
}
}
}
}

struct _erics_provider {
  template <class TInitialization, class TMemory, class T, class... TArgs>
  struct is_creatable {
    static constexpr auto value = di::concepts::creatable<TInitialization, T, TArgs...>::value;
  };

  template <class, class T, class TMemory, class... TArgs>
  auto get(const di::type_traits::direct&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T(di::type_traits::_eric_cast_t<TArgs>(args)...);
  }

  template <class, class T, class TMemory, class... TArgs>
  auto get(const di::type_traits::uniform&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T{di::type_traits::_eric_cast_t<TArgs>(args)...};
  }
};

/*<override `di` provider configuration>*/
class _erics_provider_config : public di::config {
 public:
  static auto provider(...) noexcept { return _erics_provider{}; }
};

template <typename T>
concept bool Dummy() {
  return requires(T a) {
    { a.dummy() };
  };
}

struct Dummy_impl {
  void dummy() {}
};

auto dummy_concept = [] {};

struct example {
  BOOST_DI_INJECT(example, auto t) {
    // assert(42 == i);
    std::cout << typeid(t).name() << std::endl;
    // static_assert(std::is_same<decltype(t), int>::value, "is int");
  }  //, (named = dummy_concept)Dummy d) {}
};

int main() {
  auto injector =
      di::make_injector<_erics_provider_config>(di::bind<int>.to(42), di::bind<di::_>.to(87)
                                                //, di::bind<di::_>().named(dummy_concept).to<Dummy_impl>());
                                                );

  injector.create<example>();
}

#endif

//]
