//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/scopes/shared.hpp"

#include <cassert>

namespace di = boost::di;

//<-
template <class...>
static auto& calls() {
  static auto i = 0;
  return i;
}
class ctor;
class dtor;

struct interface1 {
  virtual ~interface1() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation1 : interface1 {
  implementation1() { calls<implementation1, ctor>()++; }
  implementation1(const implementation1&) { calls<implementation1, ctor>()++; }
  implementation1(implementation1&&) { calls<implementation1, ctor>()++; }
  ~implementation1() { calls<implementation1, dtor>()++; }
  void dummy() override {}
};
struct implementation2 : interface1 {
  implementation2() { calls<implementation2, ctor>()++; }
  implementation2(const implementation2&) { calls<implementation2, ctor>()++; }
  implementation2(implementation2&&) { calls<implementation2, ctor>()++; }
  ~implementation2() { calls<implementation2, dtor>()++; }
  void dummy() override {}
};
struct implementation3 : interface1 {
  implementation3() { calls<implementation3, ctor>()++; }
  implementation3(const implementation3&) { calls<implementation3, ctor>()++; }
  implementation3(implementation3&&) { calls<implementation3, ctor>()++; }
  ~implementation3() { calls<implementation3, dtor>()++; }
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
    auto injector = di::make_injector<di::extension::shared_config>(
      di::bind<interface1>().to<implementation1>().in(di::extension::shared)  // di::unique will fail (sp1 == sp2)
    , di::bind<interface1>().to<implementation2>().named(name).in(di::extension::shared)
    );
    // clang-format on
    injector.create<example>();
  }

  {
    // clang-format off
    di::injector<std::shared_ptr<interface1>> injector =
      di::make_injector<di::extension::shared_config>(
        di::bind<interface1>().to<implementation1>().in(di::extension::shared)
    );
    // clang-format on

    injector.create<std::shared_ptr<interface1>>();
    // injector.create<std::unique_ptr<interface1>>(); // error, std::unique_ptr<interface1> is not exposed
  }

  {
    // clang-format off
    di::injector<BOOST_DI_EXPOSE((named = name)std::shared_ptr<interface1>), std::shared_ptr<interface1>> injector =
        di::make_injector<di::extension::shared_config>(
        di::bind<interface1>().to<implementation1>().in(di::extension::shared).named(name)
      , di::bind<interface1>().to<implementation2>().in(di::extension::shared).named(name)[di::override]
      , di::bind<interface1>().to<implementation1>().in(di::extension::shared)
    );
    // clang-format on

    injector.create<example>();
  }

  {
    // clang-format off
    auto injector1 = di::make_injector<di::extension::shared_config>(di::bind<interface1>().to<implementation1>().in(di::extension::shared));
    // clang-format on

    auto i1 = injector1.create<std::shared_ptr<interface1>>();
    auto injector2 = di::make_injector<di::extension::shared_config>(std::move(injector1));
    // check whether shared scope was successfully moved
    assert(i1 == injector2.create<std::shared_ptr<interface1>>());
  }

  calls<implementation1, ctor>() = 0;
  calls<implementation1, dtor>() = 0;
  calls<implementation2, ctor>() = 0;
  calls<implementation2, dtor>() = 0;
  calls<implementation3, ctor>() = 0;
  calls<implementation3, dtor>() = 0;
  {
    // clang-format off
    const auto injector = di::make_injector<di::extension::shared_config>(
       di::bind<implementation1>().in(di::singleton)          /// explicit singleton scope
     , di::bind<implementation2>().in(di::extension::shared)  /// explicit shared scope
     , di::bind<interface1>().to<implementation3>()           /// deduced shared scope
    );
    // clang-format on

    injector.create<implementation1>();
    injector.create<implementation2>();
    assert(dynamic_cast<interface1*>(injector.create<std::shared_ptr<interface1>>().get()));

    assert((calls<implementation1, ctor>() == 2));
    assert((calls<implementation1, dtor>() == 1));

    assert((calls<implementation2, ctor>() == 2));
    assert((calls<implementation2, dtor>() == 1));

    assert((calls<implementation3, ctor>() == 1));
    assert((calls<implementation3, dtor>() == 0));

  }  // injector dies
  assert((calls<implementation1, ctor>() == 2));
  assert((calls<implementation1, dtor>() == 1));  // not destroyed

  assert((calls<implementation2, ctor>() == 2));
  assert((calls<implementation2, dtor>() == 2));  // explicit scope destroyed with injector

  assert((calls<implementation3, ctor>() == 1));
  assert((calls<implementation3, dtor>() == 1));  // deduced scope destroyed with config/injector
}
