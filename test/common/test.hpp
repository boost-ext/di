//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
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

#if !defined(TEST_DEFAULT_REPORTER)
    #define TEST_DEFAULT_REPORTER "common/text_reporter.sh"
#endif

static std::map<void*, std::vector<std::string>> results;

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

struct test {
    template<class Test>
    test(const Test& test) {
        results[this].push_back("run");
        test();
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
    for (const auto& result : results) {
        if (result.second.size() > 1) {
            std::stringstream str;
            str << reporter << " \"" << argv[0] << "\" \"" << result.first << "\"";
            for (const auto& error : result.second) {
                std::system((str.str() + " " + error).c_str());
            }
            success = false;
        }
    }
    return !success;
}

#endif

