//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/scopes/session.hpp"

#include <cassert>

namespace di = boost::di;

//<-
struct interface1 {
  virtual ~interface1() noexcept = default;
  virtual void dummy() = 0;
};

struct implementation1 : interface1 {
  void dummy() override {}
};
//->

auto my_session = [] {};

int main() {
  {
    // clang-format off
    auto injector = di::make_injector(
      di::bind<interface1>().to<implementation1>().in(di::extension::session(my_session))
    );
    // clang-format on
    assert(!injector.create<std::shared_ptr<interface1>>());

    {
      auto ms = di::extension::session(my_session)();
      assert(injector.create<std::shared_ptr<interface1>>());
    }  // end of my_session

    assert(!injector.create<std::shared_ptr<interface1>>());
  }

  {
    // clang-format off
    di::injector<std::shared_ptr<interface1>> injector =
      di::make_injector(
        di::bind<interface1>().to<implementation1>().in(di::extension::session(my_session))
      );
    // clang-format on
    assert(!injector.create<std::shared_ptr<interface1>>());

    {
      auto ms = di::extension::session(my_session)();
      assert(injector.create<std::shared_ptr<interface1>>());
    }  // end of my_session

    assert(!injector.create<std::shared_ptr<interface1>>());
  }
}
