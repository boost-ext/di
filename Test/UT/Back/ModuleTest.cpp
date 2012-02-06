//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include "QDeps/Back/Module.hpp"
#include "Test/Common/Bindings.hpp"
#include "Test/Common/Ctors.hpp"

namespace QDeps
{
namespace Back
{
namespace UT
{

using namespace Test::Common;

TEST(Module, CreateUsingCopy)
{
    //given
    const int l_value = 0;
    Module<BindImplOne<l_value>::type> l_module;

    //when
    ConstRefI l_tbs = l_module.create<ConstRefI>();

    //then
    EXPECT_EQ(l_value, l_tbs.get());
}

TEST(Module, CreateUsingRef)
{
    //given
    const int l_value = 0;
    Module<BindImplOne<l_value>::type> l_module;

    //when
    ConstRefI& l_tbs = l_module.create<ConstRefI &>();

    //then
    EXPECT_EQ(l_value, l_tbs.get());
}

TEST(Module, CreateUsingConstRef)
{
    //given
    const int l_value = 0;
    Module<BindImplOne<l_value>::type> l_module;

    //when
    const ConstRefI& l_tbs = l_module.create<const ConstRefI&>();

    //then
    EXPECT_EQ(l_value, l_tbs.get());
}

TEST(Module, CreateUsingSharedPtr)
{
    //given
    const int l_value = 0;
    Module<BindImplOne<l_value>::type> l_module;

    //when
    boost::shared_ptr<ConstRefI> l_tbs = l_module.create<boost::shared_ptr<ConstRefI> >();

    //then
    EXPECT_EQ(l_value, l_tbs->get());
}

TEST(Module, CreateSharedPtrISharedPtrI)
{
    //given
    const int l_value1 = 1;
    const int l_value2 = 2;
    Module<BindImplTwo<l_value1, l_value2>::type> l_module;

    //when
    boost::shared_ptr<SharedPtrISharedPtrI> l_tbs = l_module.create<boost::shared_ptr<SharedPtrISharedPtrI> >();

    //then
    EXPECT_EQ(l_value1, l_tbs->get1());
    EXPECT_EQ(l_value2, l_tbs->get2());
}

TEST(Module, CreateRecurrentSimple)
{
    //given
    const int l_value1 = 1;
    const int l_value2 = 2;
    Module<BindRecurrentSimple<l_value1, l_value2>::type> l_module;

    //when
    boost::shared_ptr<LevelFirst> l_tbs = l_module.create<boost::shared_ptr<LevelFirst> >();

    //then
    EXPECT_EQ(l_value1, l_tbs->level()->getLevel());
    EXPECT_EQ(l_value2, l_tbs->level()->get());
}

} // namespace UT
} // namespace Back
} // namespace QDeps

