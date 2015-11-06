//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[logger]
//<-
#ifndef BOOST_DI_EXAMPLE_CPP_14_TUTORIAL_LOGGER_HPP
#define BOOST_DI_EXAMPLE_CPP_14_TUTORIAL_LOGGER_HPP

#include <iostream>
#include "ilogger.hpp"
//->

class logger : public ilogger {
 public:
  void log(const std::string& text) override { std::clog << text << std::endl; }
};

//<-
#endif
//->

//]
