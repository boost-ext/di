//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
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
  controller(const std::shared_ptr<view>& view, const std::shared_ptr<model>& model) : view_(view), model_(model) {}

  int run();

 private:
  std::shared_ptr<view> view_;
  std::shared_ptr<model> model_;
};

//<-
#endif
//->

//]
