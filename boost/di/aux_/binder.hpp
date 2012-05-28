//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_BINDER_HPP
#define BOOST_DI_AUX_BINDER_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/min_max.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/filter_view.hpp>

#include "boost/di/aux_/dependency.hpp"
#include "boost/di/aux_/make_plain.hpp"
#include "boost/di/aux_/value_type.hpp"
#include "boost/di/scopes/per_request.hpp"

namespace boost {
namespace di {
namespace aux_ {

namespace detail {

template<typename T>
struct bind
{
    typedef typename T::bind type;
};

template<typename TCallStack, typename TContext>
struct equal_call_stack
    : mpl::equal<
        mpl::iterator_range<
            typename mpl::advance<
                typename mpl::begin<TCallStack>::type
              , typename mpl::max<
                    mpl::int_<0>
                  , mpl::minus<mpl::size<TCallStack>, mpl::size<TContext> >
                >::type
            >::type
          , typename mpl::end<TCallStack>::type
        >
      , TContext
      >
{ };

template<typename T>
struct make_context
    : mpl::fold<
          typename T::context
        , mpl::vector0<>
        , mpl::if_<
              mpl::is_sequence<mpl::_2>
            , mpl::push_back<mpl::_1, mpl::_2>
            , mpl::push_back<mpl::_1, mpl::vector1<mpl::_2> >
          >
      >::type
{ };

template<
    typename TCallStack
  , typename TContext
>
struct for_each_context
    : mpl::fold<
          TContext
        , mpl::int_<equal_call_stack<TCallStack, TContext>::value>
        , mpl::if_<
              equal_call_stack<TCallStack, mpl::_2>
            , mpl::next<mpl::_1>
            , mpl::_1
          >
      >::type
{ };

template<typename TContext>
struct get_longest_context_size
    : mpl::fold<
          TContext
        , mpl::int_<0>
        , mpl::max<mpl::_1, mpl::size<mpl::_2> >
      >::type
{ };

template<typename T1, typename T2>
struct less_context_size
    : mpl::less<
         get_longest_context_size<make_context<T2> >
       , get_longest_context_size<make_context<T1> >
      >
{ };

template<
    typename TCallStack
  , typename TSeq
  , typename TCond
>
struct sort_dependencies_by_call_stack_order
    : mpl::sort<
          typename mpl::fold<
              mpl::filter_view<TSeq, has_context<mpl::_1> >
            , mpl::vector0<>
            , mpl::if_<
                  TCond
                , mpl::push_back<mpl::_1, mpl::_2>
                , mpl::_1
              >
          >::type
        , less_context_size<mpl::_1, mpl::_2>
      >::type
{ };

template<
    typename TCallStack
  , typename TSeq
  , typename TDefault
  , typename TCond =
        for_each_context<
            TCallStack
          , make_context<mpl::_2>
        >
>
struct get_dependency_by_call_stack_order
    : mpl::deref<
          mpl::begin<
              typename mpl::push_back<
                  typename sort_dependencies_by_call_stack_order<
                      TCallStack
                    , TSeq
                    , TCond
                  >::type
                , TDefault
              >::type
          >
      >::type
{ };

template<
    typename TCallStack
  , typename TSeq
  , typename TDefault
>
struct get_dependency_by_call_stack_order2
    : mpl::deref<
          mpl::begin<
              typename mpl::push_back<
                  typename sort_dependencies_by_call_stack_order<
                      TCallStack
                    , TSeq
        , for_each_context<
            TCallStack
          , make_context<mpl::_2>
        >
                  >::type
                , TDefault
              >::type
          >
      >::type
{ };

struct empty_context
{
    typedef mpl::vector0<> context;
};

template<
    typename TGiven
  , typename TExternals
  , typename TCallStack
  , typename TDefault
>
struct make_default_dependency
    : TDefault::template rebind<
          TGiven
        , typename value_type<TGiven>::type
        , typename get_dependency_by_call_stack_order2<
              TCallStack
            , TExternals
            , empty_context
          >::type::context
      >
{ };

template<typename TBind, typename T>
struct comparator
    : mpl::apply<TBind, T>::type
{ };

} // namespace detail

template<
    typename T
  , typename TCallStack
  , typename TDeps
  , typename TExternals
  , typename TDefault =
        dependency<scopes::per_request, mpl::_1, mpl::_2, mpl::_3>
  , template<
        typename
    > class TMakePlain = make_plain
>
struct binder_impl
    : detail::get_dependency_by_call_stack_order<
          TCallStack
        , TDeps
        , typename detail::make_default_dependency<
              typename TMakePlain<T>::type
            , TExternals
            , TCallStack
            , TDefault
          >::type
        , mpl::and_<
              detail::comparator<
                  detail::bind<mpl::_2>
                , typename TMakePlain<T>::type
              >
            , detail::for_each_context<
                  TCallStack
                , detail::make_context<mpl::_2>
              >
          >
      >
{ };

template<
    typename TDeps = mpl::vector0<>
  , typename TExternals = mpl::vector0<>
>
struct binder
{
    template<
        typename T
      , typename TCallStack
    >
    struct impl
        : binder_impl<T, TCallStack, TDeps, TExternals>
    { };
};

} // namespace aux_
} // namespace di
} // namespace boost

#endif

