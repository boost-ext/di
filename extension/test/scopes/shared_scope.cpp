//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/scopes/shared_scope.hpp"

#include <cassert>

//<-
//<<by default shared scope is synchronized>>
#if !defined(BOOST_DI_THREAD_SAFE_EXTENTIONS)
#define BOOST_DI_THREAD_SAFE_EXTENTIONS true
#endif

#if BOOST_DI_THREAD_SAFE_EXTENTIONS
#include <mutex>
#endif
//->

namespace di = boost::di;

//<-
struct interface1 {
  virtual ~interface1() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation1 : interface1 {
  void dummy() override {}
};
struct implementation2 : interface1 {
  void dummy() override {}
};
//->

auto name = [] {};

class example {
 public:
  BOOST_DI_INJECT(example, (named = name) std::shared_ptr<interface1> sp1, (named = name) std::shared_ptr<interface1> sp2,
                  std::shared_ptr<interface1> sp3) {
    assert(sp1 == sp2);
    assert(dynamic_cast<implementation2*>(sp1.get()));
    assert(dynamic_cast<implementation2*>(sp2.get()));
    assert(dynamic_cast<implementation1*>(sp3.get()));
  }
};

int main() {
  {
    // clang-format off
    auto injector = di::make_injector(
      di::bind<interface1>().to<implementation1>().in(di::extension::shared)  // di::unique will fail (sp1 == sp2)
    , di::bind<interface1>().to<implementation2>().named(name).in(di::extension::shared)
    );
    // clang-format on
    injector.create<example>();
  }

  {
    // clang-format off
    di::injector<std::shared_ptr<interface1>> injector =
      di::make_injector(
        di::bind<interface1>().to<implementation1>().in(di::extension::shared)
      );
    // clang-format on

    injector.create<std::shared_ptr<interface1>>();
    // injector.create<std::unique_ptr<interface1>>(); // error, std::unique_ptr<interface1> is not exposed
  }

  {
    // clang-format off
    di::injector<BOOST_DI_EXPOSE((named = name)std::shared_ptr<interface1>), std::shared_ptr<interface1>> injector =
        di::make_injector(
          di::bind<interface1>().to<implementation1>().in(di::extension::shared).named(name)
        , di::bind<interface1>().to<implementation2>().in(di::extension::shared).named(name)[di::override]
        , di::bind<interface1>().to<implementation1>().in(di::extension::shared)
        );
    // clang-format on

    injector.create<example>();
  }

  {
    // clang-format off
    auto injector1 = di::make_injector(di::bind<interface1>().to<implementation1>().in(di::extension::shared));
    // clang-format on

    auto i1 = injector1.create<std::shared_ptr<interface1>>();
    auto injector2 = di::make_injector(std::move(injector1));
    // check whether shared scope was successfully moved
    assert(i1 == injector2.create<std::shared_ptr<interface1>>());
  }
}
