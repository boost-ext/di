//
// Copyright (c) 2012-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation : interface {
  void dummy() override {}
};

/*<<to constructor extension>>*/
template <class... TCtor>
struct constructor_impl {
  template <class TInjector, class T,
            std::enable_if_t<
                boost::di::concepts::creatable<boost::di::type_traits::direct, typename T::expected, TCtor...>::value, int> = 0>
  auto operator()(const TInjector& injector, const T&) const {
    return new typename T::expected{injector.template create<TCtor>()...};
  }
};

template <class... TCtor>
struct constructor : constructor_impl<TCtor...> {};
//->

/*<<normal constructor - to_constructor binding is not needed for it>>*/
struct ctor {
  ctor(int i, std::string s, std::unique_ptr<interface> impl) {
    assert(i == 2);
    assert(s == "hello");
    assert(dynamic_cast<implementation*>(impl.get()));
  }
};

/*<<ambiguous constructor>>*/
struct ambiguous_ctor {
  ambiguous_ctor(int i, std::string s) {
    assert(i == 2);
    assert(s == "hello");
  }

  ambiguous_ctor(std::string s, int i) {
    assert(s == "hello");
    assert(i == 2);
  }
};

/*<<variadic constructor - has to be `explicit`>>*/
struct variadic {
  template <class... Ts>
  explicit variadic(Ts&&... args) {  // has to be explicit
    assert(sizeof...(Ts) == 3);
    std::tuple<Ts...> ts{std::forward<Ts>(args)...};
    assert(std::get<0>(ts) == 2);
    assert(std::get<1>(ts) == "hello");
    assert(dynamic_cast<implementation*>(std::get<2>(ts).get()));
  }
};

int main() {
  // clang-format off
  /*<<define constructor types>>*/
  auto injector = di::make_injector(
    di::bind<ctor>().to(constructor<int, std::string, std::unique_ptr<interface>>())
  , di::bind<ambiguous_ctor>().to(constructor<int, std::string>())
  , di::bind<variadic>().to(constructor<int, std::string, std::unique_ptr<interface>>())

      /*<<additional bindings>>*/
  , di::bind<interface>().to<implementation>()
  , di::bind<int>().to(2), di::bind<std::string>().to("hello")
  );
  // clang-format on

  /*<<create types using defined constructors>>*/
  injector.create<ctor>();
  injector.create<ambiguous_ctor>();
  injector.create<variadic>();
}
