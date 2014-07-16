//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_EXAMPLE_CPP_14_TUTORIAL_ILOGGER_HPP
#define BOOST_DI_EXAMPLE_CPP_14_TUTORIAL_ILOGGER_HPP

#include <string>

class ilogger {
public:
    virtual ~ilogger() { }
    virtual void log(const std::string&) = 0;
};

#endif


