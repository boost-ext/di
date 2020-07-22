//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
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
#if defined(_MSC_VER)
class Type;
class Number;
#endif
//->

template <class T = class Type, class TNumber = class Number>
struct hello {
  using type = T;
  explicit hello(const TNumber& number) : number{number} {}
  TNumber number;
};

template <class T = int>
struct world {
  using type = T;
};

int main() {
  auto i = 42;

  // clang-format off
  const auto injector = di::make_injector(
     di::bind<class Type>().to<world>()
   , di::bind<class Number>().to<int>(i)
  );
  // clang-format on

  {
    const auto hw = injector.create<hello>();

    static_assert(std::is_same<int, decltype(hw)::type::type>{}, "Type != int");
    assert(42 == hw.number);
  }

  {
    const auto hw = injector.create<std::shared_ptr<decltype(injector.create<hello>())>>();

    static_assert(std::is_same<int, decltype(hw)::element_type::type::type>{}, "Type != int");
    assert(42 == hw->number);
  }
}
