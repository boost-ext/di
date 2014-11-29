#ifndef BOOST_DI_TEST_HPP
#define BOOST_DI_TEST_HPP

#include <cassert>

#define expect(...) assert((__VA_ARGS__))
#define expect_eq(t1, t2) assert((t1 == t2))
#define expect_neq(t1, t2) assert((t1 != t2))

struct test_skip {
    template<typename Test>
    test_skip(const Test&) { }
};

struct test {
    template<typename Test>
    test(const Test& test) {
        test();
    }
};

int main() { }

#endif

