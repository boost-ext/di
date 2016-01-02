//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[view_hpp
//<-
#ifndef VIEW_HPP
#define VIEW_HPP

#include <memory>
#include "common.hpp"
//->

class view {
 public:
  view(std::unique_ptr<iwindow>, std::shared_ptr<ibutton>);
  void render() {}
};

//<-
#endif
//->

//]
