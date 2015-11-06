//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[ilogger]
//<-
#ifndef BOOST_DI_EXAMPLE_CPP_14_TUTORIAL_ILOGGER_HPP
#define BOOST_DI_EXAMPLE_CPP_14_TUTORIAL_ILOGGER_HPP

#include <string>
//->

class ilogger {
 public:
  virtual ~ilogger() noexcept = default;
  virtual void log(const std::string&) = 0;
};

//<-
#endif
//->

//]
