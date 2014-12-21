//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <memory>
#include <cstdlib>

#include <boost/di.hpp>

#include "ilogger.hpp"
#include "logger.hpp"

namespace di = boost::di;

class first { };
class second { };

class app {
public:
    BOOST_DI_INJECT(app, (first, int value1), (second, int value2), const std::string& text, std::shared_ptr<ilogger> logger)
        : value1_(value1), value2_(value2) , text_(text), logger_(logger)
    { }

    void run() const {
        if (value1_ || value2_) {
            logger_->log(text_);
        }
    }

private:
    int value1_ = 0;
    int value2_ = 0;
    std::string text_;
    std::shared_ptr<ilogger> logger_;
};

class module {
public:
    explicit module(int i)
        : i_(i)
    { }

    auto configure() const {
        return di::make_injector(
            di::bind<ilogger, logger>
          , di::bind<int>.named(first{}).to(i_)
          , di::bind<int>.named(second{}).to(0)
          , di::bind<std::string>.to("hello world")
        );
    }

private:
    int i_ = 0;
};

int main(int argc, char** argv) {
    auto injector = di::make_injector(module(argc > 1 ? std::atoi(argv[1]) : 0));
    injector.create<app>().run();

    return 0;
}

