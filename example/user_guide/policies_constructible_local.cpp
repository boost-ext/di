//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <type_traits>
//->
#include <boost/di.hpp>

//<-
namespace di = boost::di;
//->

class injected_and_bound : public di::config {
 public:
  static auto policies(...) noexcept {
    using namespace di::policies;
    using namespace di::policies::operators;
    // clang-format off
    return di::make_policies(
      constructible(is_injected<di::_>{} && is_bound<di::_>{})
    );
    // clang-format on
  }
};

struct T {
  BOOST_DI_INJECT(T, int, double) {}
};

int main() {
  // clang-format off
  auto injector = di::make_injector<injected_and_bound>(
    di::bind<>().to(42)
  , di::bind<>().to(87.0)
  );
  // clang-format on

  injector.create<T>();
}
