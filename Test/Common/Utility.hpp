//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_TEST_COMMON_UTILITY_HPP
#define QDEPS_TEST_COMMON_UTILITY_HPP

#include <gtest/gtest.h>

#define QDEPS_STATIC_ASSERT(cond, expr, types)              \
    throw ::QDeps::Test::Common::StaticAssert(cond, #expr)

#define EXPECT_STATIC_ASSERT(code, msg)                     \
    try                                                     \
    {                                                       \
        code;                                               \
        ASSERT_TRUE(false);                                 \
    }                                                       \
    catch(const ::QDeps::Test::Common::StaticAssert& e)     \
    {                                                       \
        EXPECT_FALSE(e.cond);                               \
        EXPECT_EQ(#msg, e.expr);                            \
    }

#define TEST_T(name, test, ...)                             \
    template<typename TInjector>                            \
    struct name##test : public ::testing::Test              \
    {                                                       \
        TInjector injector;                                 \
    };                                                      \
    typedef ::testing::Types<__VA_ARGS__> Types##test;      \
    TYPED_TEST_CASE(name##test, Types##test);               \
    TYPED_TEST(name##test, test)

namespace QDeps
{
namespace Test
{
namespace Common
{

struct StaticAssert
{
    StaticAssert(bool cond, const std::string& expr)
        : cond(cond), expr(expr)
    { }

    bool cond;
    std::string expr;
};

} // namespace Common
} // namespace Test
} // namespace QDeps

#endif

