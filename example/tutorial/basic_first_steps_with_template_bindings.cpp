//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <type_traits>
//->
#include <boost/di.hpp>

namespace di = boost::di;

class Greater;

template <class T = Greater>
struct example {
  using type = T;
};

struct hello {};

int main() {
  // clang-format off
  const auto injector = di::make_injector(
    di::bind<Greater>().to<hello>()
  );
  // clang-format on

  auto object = injector.create<example>();
  static_assert(std::is_same<hello, decltype(object)::type>{}, "");
}
