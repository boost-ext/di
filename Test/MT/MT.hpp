#ifndef QDEPS_TEST_MT_HPP
#define QDEPS_TEST_MT_HPP

#include <boost/preprocessor/cat.hpp>
#include <gtest/gtest.h>

#define MT(...)                                                         \
    namespace BOOST_PP_CAT(MT, __LINE__)                                \
    {                                                                   \
        __VA_ARGS__                                                     \
    }                                                                   \
    TEST(QDeps, BOOST_PP_CAT(Test, BOOST_PP_CAT(MT, __LINE__)))         \
    {                                                                   \
        using namespace BOOST_PP_CAT(MT, __LINE__);

#endif

