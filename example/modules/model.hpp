//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#ifndef MODEL_HPP
#define MODEL_HPP

#include <boost/di.hpp>
#include <memory>
#include "common.hpp"
//->

namespace di = boost::di;

static auto rows = [] {};
static auto colls = [] {};

class model {
 public:
  BOOST_DI_INJECT(model, (named = rows) int, (named = colls) int, std::unique_ptr<iboard>);
  void update() {}
};

//<-
#endif
//->
