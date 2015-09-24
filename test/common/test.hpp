//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TEST_HPP
#define BOOST_DI_TEST_HPP

#if defined(_MSC_VER)
    #define __has_include(...) 0
#endif

#undef NDEBUG
#include <cassert>

#define expect(...) assert((__VA_ARGS__) && "fail")
#define static_expect(...) static_assert((__VA_ARGS__), "fail")

struct test {
    template<class Test>
    test(const Test& test) {
        test();
    }
};

int main() { }

#endif

