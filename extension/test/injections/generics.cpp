//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injections/generics.hpp"

#include <cassert>

namespace di = boost::di;

/*<<generic interface>>*/
struct interface {
  GENERIC(interface) {}
  bool update(int i) REQUIRES(update, i);
};

/*<<implementation without inheritance>>*/
struct implementation1 {
  bool update(int i) { return i == 42; }
};

/*<<implementation without inheritance>>*/
struct implementation2 {
  bool update(int i) { return i != 42; }
};

struct example {
  BOOST_DI_INJECT(example, interface i) : i(i) {}
  interface i;
};

int main() {
  /*<override `di` provider configuration>*/
  struct generics_provider_config : di::config {
    static auto provider(...) noexcept { return di::extension::generics_provider{}; }
  };

  {
    // clang-format off
    const auto injector = di::make_injector<generics_provider_config>(
      di::bind<interface>().to<implementation1>()
    );
    // clang-format on

    auto object = injector.create<example>();
    assert(!object.i.update(0));  /// calls implementation1.update(0)
    assert(object.i.update(42));  /// calls implementation1.update(42)
  }

  {
    // clang-format off
    const auto injector = di::make_injector<generics_provider_config>(
      di::bind<interface>().to<implementation2>()
    );
    // clang-format on

    auto object = injector.create<example>();
    assert(object.i.update(0));    /// calls implementation2.update(0)
    assert(!object.i.update(42));  /// calls implementation2.update(42)
  }
}
