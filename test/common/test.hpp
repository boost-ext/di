//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TEST_HPP
#define BOOST_DI_TEST_HPP

#include <cassert>

#define expect(...) assert((__VA_ARGS__))
#define expect_eq(t1, t2) assert((t1 == t2))
#define expect_neq(t1, t2) assert((t1 != t2))

struct test {
    template<typename Test>
    test(const Test& test) {
        test();
    }
};

struct test_skip {
    template<typename Test>
    test_skip(const Test&) { }
};

int main() { }

#endif

