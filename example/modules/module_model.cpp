//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//<-
#include "module_model.hpp"
#include <boost/di.hpp>
#include "common.hpp"
#include "config.hpp"
#include "model.hpp"
//->

namespace di = boost::di;

di::injector<model&> module_model() noexcept {
  // clang-format off
  return di::make_injector(
    di::bind<iboard>().to<board>()
  , di::bind<int>().named(rows).to(42)
  , di::bind<int>().named(colls).to(42)
  );
  // clang-format on
}
