//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <cassert>

namespace di = boost::di;

struct iworld {
  virtual ~iworld() noexcept = default;
  virtual void f() = 0;
};

struct world : iworld {
  void f() override {}
};

struct hello {
  explicit hello(iworld&) {}
};

int main() {
  {
    const auto injector = di::make_injector(di::bind<iworld>().to<world>());
    static_assert(di::is_creatable<hello>(injector), "");
  }

  {
    const auto injector = di::make_injector();
    static_assert(!di::is_creatable<hello>(injector), "iworld is not abstract!");
  }
}
