//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <memory>
#include "fake_logger.hpp"
#include "app.hpp"

void test_dont_print_hello_world_when_value_equal_0() {
    //given
    const auto fake_logger_ = std::make_shared<fake_logger>();

    //when
    app(fake_logger_, 0).run();

    //then
    assert(0 == fake_logger_->log_calls_.size());
}

void test_print_hello_world_when_value_not_equal_to_0() {
    //given
    const std::string hello_world = "hello world";
    const auto fake_logger_ = std::make_shared<fake_logger>();

    //when
    app(fake_logger_, 1).run();

    //then
    assert(1 == fake_logger_->log_calls_.size());
    assert(hello_world == fake_logger_->log_calls_[0]);
}

int main() {
    test_dont_print_hello_world_when_value_equal_0();
    test_print_hello_world_when_value_not_equal_to_0();
    return 0;
}

