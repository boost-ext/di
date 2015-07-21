//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TEST_HPP
#define BOOST_DI_TEST_HPP

#include <map>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <functional>
#include <type_traits>

#if !defined(TEST_DEFAULT_REPORTER)
    #if defined(_MSC_VER)
        #define TEST_DEFAULT_REPORTER "common\\text_reporter.bat"
    #else
        #define TEST_DEFAULT_REPORTER "common/text_reporter.sh"
    #endif
#endif

static std::map<void*, std::vector<std::string>> results;
static std::vector<std::function<void()>> tests;
static std::vector<std::function<void(const char*)>> tests_with_progname;

#define HAS_METHOD(name, call_name)                                 \
    template<class T, class... TArgs>                               \
    decltype(std::declval<T>().call_name(std::declval<TArgs>()...)  \
           , std::true_type())                                      \
    has_##name##_impl(int);                                         \
                                                                    \
    template<class, class...>                                       \
    std::false_type has_##name##_impl(...);                         \
                                                                    \
    template<class T, class... TArgs>                               \
    struct has_##name : decltype(has_##name##_impl<T, TArgs...>(0)) \
    { }

HAS_METHOD(call_operator, operator());

#define expect(...) \
    do { \
        if (!(__VA_ARGS__)) { \
            std::stringstream error; \
            error << __FILE__ << " " << __LINE__ << " expect \"" << #__VA_ARGS__ << "\""; \
            results.rbegin()->second.push_back(error.str()); \
        } \
    } while (false)

#define expect_eq(t1, t2) \
    do { \
        if (t1 != t2) { \
            std::stringstream error; \
            error << __FILE__ << " " << __LINE__ << " expect_eq \"" << #t1 << "\" \"" << #t2 << "\""; \
            results.rbegin()->second.push_back(error.str()); \
        } \
    } while (false)

#define expect_neq(t1, t2) \
    do { \
        if (t1 == t2) { \
            std::stringstream error; \
            error << __FILE__ << " " << __LINE__ << " expect_neq \"" << #t1 << "\" \"" << #t2 << "\""; \
            results.rbegin()->second.push_back(error.str()); \
        } \
    } while (false)

#define static_expect(...) static_assert((__VA_ARGS__), "")

struct test {
    template<class Test>
    test(const Test& test) {
        results[this].push_back("run");
        add_test(test, has_call_operator<Test, const char*>{});
    }

    template<class Test>
    void add_test(const Test& test, const std::true_type&) {
        tests_with_progname.push_back(test);
    }

    template<class Test>
    void add_test(const Test& test, const std::false_type&) {
        tests.push_back(test);
    }
};

struct test_skip {
    template<class Test>
    test_skip(const Test&) {
        results[this].push_back("skip");
    }
};

template<class T>
struct test_type {
    using type = T;
};

int main(int argc, char** argv) {
    auto success = true;
    auto reporter = argc > 1 ? argv[1] : TEST_DEFAULT_REPORTER;

    for (const auto& test : tests) {
        test();
    }

    for (const auto& test : tests_with_progname) {
        test(argv[0]);
    }

    for (const auto& result : results) {
        if (result.second.size() > 1) {
            std::stringstream str;
            str << reporter << " \"" << argv[0] << "\" \"" << result.first << "\"";
            for (const auto& error : result.second) {
                if (std::system((str.str() + " " + error).c_str())) {
                    str << reporter << " \"" << argv[0] << "\" \"" << "error" << "\"";
                }
            }
            success = false;
        }
    }
    return !success;
}

#endif

