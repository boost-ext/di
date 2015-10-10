//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <string>
#include <iostream>
#include <memory>
#include <cstdlib>

#include <boost/di.hpp>

#include "ilogger.hpp"
#include "logger.hpp"

namespace di = boost::di;

class app {
public:
    app(int value, const std::string& text, std::shared_ptr<ilogger> logger)
        : value_(value), text_(text), logger_(logger)
    { }

    void run() const {
        if (value_) {
            logger_->log(text_);
        }
    }

private:
    int value_ = 0;
    std::string text_;
    std::shared_ptr<ilogger> logger_;
};

auto module(int i) {
    return di::make_injector(
        di::bind<ilogger>().to<logger>()
      , di::bind<int>().to(i)
      , di::bind<std::string>().to("hello world")
    );
};

int main(int argc, char** argv) {
    auto injector = di::make_injector(module(argc > 1 ? std::atoi(argv[1]) : 0));
    injector.create<app>().run();
}

