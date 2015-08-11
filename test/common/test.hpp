//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TEST_HPP
#define BOOST_DI_TEST_HPP

#include <iostream>

#define expect(...) \
    do { \
        if (!(__VA_ARGS__)) { \
            std::cerr << __FILE__ << " " << __LINE__ << " expect \"" << #__VA_ARGS__ << "\"" << std::endl; \
            std::abort(); \
        } \
    } while (false)

#define expect_eq(t1, t2) \
    do { \
        if (t1 != t2) { \
            std::cerr << __FILE__ << " " << __LINE__ << " expect_eq \"" << #t1 << "\" \"" << #t2 << "\"" << std::endl; \
            std::abort(); \
        } \
    } while (false)

#define expect_neq(t1, t2) \
    do { \
        if (t1 == t2) { \
            std::cerr << __FILE__ << " " << __LINE__ << " expect_neq \"" << #t1 << "\" \"" << #t2 << "\"" << std::endl; \
            std::abort(); \
        } \
    } while (false)

#define static_expect(...) \
    static_assert((__VA_ARGS__), "static_expect")

struct test {
    template<class Test>
    test(const Test& test) {
        test();
    }
};

struct test_skip {
    template<class Test>
    test_skip(const Test&) { }
};

int main() { }

#endif

