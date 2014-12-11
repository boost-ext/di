//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <memory>

#include <boost/di.hpp>
#include <mocks_injector.hpp>

#include "ilogger.hpp"

namespace di = boost::di;

class app {
public:
    app(int value, const std::string& text, std::unique_ptr<ilogger> logger)
        : value_(value), text_(text), logger_(std::move(logger))
    { }

    void run() const {
        if (value_) {
            logger_->log(text_);
        }
    }

private:
    int value_ = 0;
    std::string text_;
    std::unique_ptr<ilogger> logger_;
};

int main() {
    const std::string hello_world = "hello world";

    auto test_dont_log_hello_world_when_value_equals_0 = [&] {
        auto mi = di::make_mocks_injector(
            di::bind<int>::to(0)
        );

        mi.create<std::unique_ptr<app>>()->run();
    };

    auto test_log_hello_world_when_value_equals_1 = [&] {
        auto mi = di::make_mocks_injector(
            di::bind<int>::to(1)
          , di::bind<std::string>::to(hello_world)
        );

        EXPECT_CALL(mi, ilogger::log).With(hello_world);

        mi.create<std::unique_ptr<app>>()->run();
    };

    // run tests
    test_dont_log_hello_world_when_value_equals_0();
    test_log_hello_world_when_value_equals_1();

    return 0;
}

