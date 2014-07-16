//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_EXAMPLE_CPP_14_TUTORIAL_FAKE_LOGGER_HPP
#define BOOST_DI_EXAMPLE_CPP_14_TUTORIAL_FAKE_LOGGER_HPP

#include <vector>
#include "ilogger.hpp"

class fake_logger : public ilogger {
public:
    void log(const std::string& text) override {
        log_calls_.push_back(text);
    }

    std::vector<std::string> log_calls_;
};

#endif

