//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_TEST_COMMON_UTILITY_HPP
#define QDEPS_TEST_COMMON_UTILITY_HPP

#include <gtest/gtest.h>
#include <typeinfo>
#include <vector>
#include <string>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>

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

template<typename TSeq>
class Visitor
{
    typedef std::vector<std::string> Visits;

public:

    ~Visitor()
    {
        verify<TSeq>(0);
    }

    template<typename T, typename TCallStack, typename TScope> void operator()() const
    {
        visits.push_back(typeid(T).name());
    }

private:
    template<typename Seq> void verify(int, typename boost::enable_if< boost::mpl::empty<Seq> >::type* = 0) { }
    template<typename Seq> void verify(int i, typename boost::disable_if< boost::mpl::empty<Seq> >::type* = 0)
    {
        EXPECT_EQ(typeid(typename boost::mpl::front<Seq>::type).name(), visits.at(i));
        verify<typename boost::mpl::pop_front<Seq>::type>(i + 1);
    }

    mutable Visits visits;
};

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

