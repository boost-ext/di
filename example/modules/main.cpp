//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include "config.hpp"
#include "controller.hpp"
#include "module_model.hpp"
#include "module_view.hpp"
//->

namespace di = boost::di;

int main() {
  auto injector = di::make_injector(module_view(), module_model());
  return injector.create<controller>().run();
}
