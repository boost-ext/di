//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <memory>
//->
class custom_policy;
#define BOOST_DI_CFG custom_policy
#include <boost/di.hpp>

namespace di = boost::di;

class custom_policy : public di::config {
 public:
  static auto policies(...) noexcept {
    using namespace di::policies;
    using namespace di::policies::operators;
    return di::make_policies(constructible(is_bound<di::_>{}));
  }
};

struct example {
  example(int, double) {}
};

int main() {
  /*<<create shared_ptr `example` with per injector policy setting>>*/
  {
    // clang-format off
    auto injector = di::make_injector<custom_policy>(
      di::bind<int>().to(42)
    , di::bind<double>().to(87.0)
    );
    // clang-format on

    injector.create<example>();
  }

  /*<<create shared_ptr `example` with global policy setting>>*/
  {
    // clang-format off
    auto injector = di::make_injector(
      di::bind<int>().to(42)
    , di::bind<double>().to(87.0)
    );
    // clang-format on

    injector.create<example>();
  }
}
