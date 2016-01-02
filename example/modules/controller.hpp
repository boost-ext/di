//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[controller_hpp
//<-
#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <memory>
#include "model.hpp"
#include "view.hpp"
//->

struct controller {
  controller(view& view, model& model) : view_(view), model_(model) {}
  int run();

 private:
  view& view_;
  model& model_;
};

//<-
#endif
//->

//]
