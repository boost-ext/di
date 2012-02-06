//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_TEST_COMMON_BINDINGS_HPP
#define QDEPS_TEST_COMMON_BINDINGS_HPP

#include "Test/Common/Interfaces.hpp"
#include "Test/Common/Ctors.hpp"

namespace QDeps
{
namespace Test
{
namespace Common
{

using namespace Back::Aux;
using namespace Back::Aux::Scopes;

template<int Value>
struct BindImplOne
{
    typedef boost::mpl::vector
    <
        Impl<PerRequest, I<Value>, C<Value> >
    >
    type;
};

template<int Value1, int Value2>
struct BindImplTwo
{
    typedef boost::mpl::vector
    <
        Impl<PerRequest, I<Value1>, C<Value1> >,
        Impl<PerRequest, I<Value2>, C<Value2> >
    >
    type;
};

template<int Value1, int Value2>
struct BindRecurrentSimple
{
    typedef boost::mpl::vector
    <
        Impl<PerRequest, ILevel, LevelSecond<Value2, Value1> >,
        Impl<PerRequest, I<Value2>, C<Value2> >
    >
    type;
};

} // namespace Common
} // namespace Test
} // namespace QDeps

#endif

