//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include <cassert>
#include <boost/di.hpp>

namespace di = boost::di;

template <int N>
using int_ = std::integral_constant<int, N>;

int main() {
  auto injector = di::make_injector(di::bind<int>().to<int_<42>>());

  assert(42 == injector.create<int>());
}
