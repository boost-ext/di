//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include "model.hpp"
#include <boost/di.hpp>
#include <cassert>
//->

model::model(int rows, int colls, std::unique_ptr<iboard> b) {
  assert(rows == 42);
  assert(colls == 42);
  assert(b.get());
}
