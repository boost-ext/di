//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
//->
#include <boost/di.hpp>

namespace di = boost::di;

using Pointer = int;

auto module(Pointer* ptr) { return di::bind<Pointer>().to(*ptr); }

struct app {
  app(Pointer& ptr) { assert(42 == ptr); }
};

int main() {
  di::aux::owner<Pointer*> ptr{new Pointer{42}};

  auto injector = di::make_injector(module(ptr));
  injector.create<app>();

  delete ptr;
}
