//
// Copyright (c) 2012-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

namespace di = boost::di;

test should_throw_when_ctor_throws = [] {
  struct except {
    except() { throw 0; }
  };

  const auto injector = di::make_injector();

  auto cought = false;
  try {
    injector.create<except>();
  } catch (...) {
    cought = true;
  }
  expect(cought);
};
