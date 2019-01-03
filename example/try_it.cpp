//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <cassert>
#include <iostream>

namespace di = boost::di;

//<-
#if defined(_MSC_VER)
class Greater;
#endif
//->

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
template <class T = class Greater>
struct example {
  T h;
  iworld& w;
};

int main() {
  // clang-format off
  const auto injector = di::make_injector(
     di::bind<iworld>().to<world>()    // bind interface to implementation
   , di::bind<>().to(42)               // bind int to value 42
   , di::bind<class Greater>().to<hello>()   // bind template to type
  );
  // clang-format off

  /*<<create `example`>>*/
  injector.create<example>(); // or di::create<example>(injector)
}
