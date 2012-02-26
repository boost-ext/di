//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_BINDING_HPP
#define QDEPS_BACK_BINDING_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/size.hpp>
#include "QDeps/Back/Utility.hpp"

namespace QDeps
{
namespace Back
{

namespace Detail
{

template<typename TCallStack, typename TContext>
struct EqualCallStack : boost::mpl::equal
    <
        boost::mpl::iterator_range
        <
            typename boost::mpl::advance_c
            <
                typename boost::mpl::begin<TCallStack>::type,
                boost::mpl::size<TCallStack>::value - boost::mpl::size<TContext>::value
            >::type,
            typename boost::mpl::end<TCallStack>::type
        >,
        TContext
    >
{ };

template<typename T1, typename T2>
struct LessContextSize : boost::mpl::bool_<(GetContextSize<T1>::value > GetContextSize<T2>::value)>::type
{ };

} // namespace Detail

template<typename T, typename TCallStack, typename TDeps>
struct Binding : boost::mpl::sort
    <
        typename boost::mpl::fold
        <
            TDeps,
            boost::mpl::vector0<>,
            boost::mpl::if_
            <
                boost::mpl::and_
                <
                    boost::is_same<T, GetDependency<boost::mpl::_2> >,
                    Detail::EqualCallStack<TCallStack, GetContext<boost::mpl::_2> >
                >,
                boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>,
                boost::mpl::_1
            >
        >::type,
        Detail::LessContextSize<boost::mpl::_1, boost::mpl::_2>
    >::type
{ };

} // namespace Back
} // namespace QDeps

#endif

