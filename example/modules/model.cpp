//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <boost/di.hpp>
#include "model.hpp"
//->

model::model(int rows, int colls, std::unique_ptr<iboard> b) {
  assert(rows == 42);
  assert(colls == 42);
  assert(b.get());
}
