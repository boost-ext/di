//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include "module_view.hpp"
#include <boost/di.hpp>
#include "common.hpp"
#include "config.hpp"
#include "view.hpp"
//->

namespace di = boost::di;

di::injector<view&> module_view() noexcept {
  // clang-format off
  return di::make_injector(
    di::bind<ibutton>().to<button>()
  , di::bind<iwindow>().to<window>()
  );
  // clang-format on
}
