//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_DETAIL_BINDER_HPP
#define QDEPS_BACK_DETAIL_BINDER_HPP

#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/begin_end.hpp>
#include "QDeps/Back/Aux/Utility.hpp"

namespace QDeps
{
namespace Back
{
namespace Detail
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
struct LessContextSize : boost::mpl::bool_<(Aux::GetContextSize<T1>::value > Aux::GetContextSize<T2>::value)>::type
{ };

template<typename T, typename TBind>
struct Comparator : boost::mpl::apply<TBind, T>::type
{ };

} // namespace Detail

template
<
    typename T,
    typename TCallStack,
    typename TDeps,
    typename TDefault
>
struct Binder : boost::mpl::deref
    <
        boost::mpl::begin
        <
            typename boost::mpl::push_back
            <
                typename boost::mpl::sort
                <
                    typename boost::mpl::fold
                    <
                        TDeps,
                        boost::mpl::vector0<>,
                        boost::mpl::if_
                        <
                            boost::mpl::and_
                            <
                                Detail::Comparator<T, Aux::GetBind<boost::mpl::_2> >,
                                Detail::EqualCallStack<TCallStack, Aux::GetContext<boost::mpl::_2> >
                            >,
                            boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>,
                            boost::mpl::_1
                        >
                    >::type,
                    Detail::LessContextSize<boost::mpl::_1, boost::mpl::_2>
                >::type,
                typename boost::mpl::apply<TDefault, T>::type
            >::type
        >
    >::type
{ };

} // namespace Detail
} // namespace Back
} // namespace QDeps

#endif

