//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_BACK_DETAIL_BINDER_HPP
#define DI_BACK_DETAIL_BINDER_HPP

#include <boost/utility/enable_if.hpp>
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
#include <boost/mpl/has_xxx.hpp>
#include "di/back/aux/utility.hpp"

namespace di
{
namespace back
{
namespace detail
{

namespace detail
{

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

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
struct LessContextSize : boost::mpl::bool_<(aux::get_context_size<T1>::value > aux::get_context_size<T2>::value)>::type
{ };

template<typename T, typename TBind>
struct Comparator : boost::mpl::apply<TBind, T>::type
{ };

template<typename T, typename TDefault, typename = void>
struct MakeDefaultDependency
    : TDefault::template Rebind<typename aux::make_plain<T>::type, typename aux::make_plain<T>::type>
{ };

template<typename T, typename TDefault>
struct MakeDefaultDependency<T, TDefault, typename boost::enable_if<has_element_type<T> >::type>
    : TDefault::template Rebind<typename aux::make_plain<T>::type, typename aux::make_plain<typename T::value_type>::type>
{ };

} // namespace detail

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
                                detail::Comparator<typename aux::make_plain<T>::type, aux::get_bind<boost::mpl::_2> >,
                                detail::EqualCallStack<TCallStack, aux::get_context<boost::mpl::_2> >
                            >,
                            boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>,
                            boost::mpl::_1
                        >
                    >::type,
                    detail::LessContextSize<boost::mpl::_1, boost::mpl::_2>
                >::type,
                typename detail::MakeDefaultDependency<T, TDefault>::type
            >::type
        >
    >::type
{ };

} // namespace detail
} // namespace back
} // namespace di

#endif

