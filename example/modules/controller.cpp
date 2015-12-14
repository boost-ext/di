//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[controller_cpp
//<-
#include "controller.hpp"
//->

int controller::run() {
  model_.update();
  view_.render();
  return 0;
}

//]
