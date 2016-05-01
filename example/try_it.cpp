//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <iostream>
#include <string>
#include <boost/di.hpp>

namespace di = boost::di;

struct iworld {
  virtual ~iworld() noexcept = default;
};
struct world : iworld {
  world() { std::cout << " world!" << std::endl; }
};

struct hello {
  explicit hello(int i) {
    assert(42 == i);
    std::cout << "hello";
  }
};

/// aggregate initialization `example{hello, world}`
struct example {
  hello h;
  iworld& w;
};

int main() {
  auto injector = di::make_injector(di::bind<iworld>().to<world>(), di::bind<>().to(42));
  injector.create<example>();
}
