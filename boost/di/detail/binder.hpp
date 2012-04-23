//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_BINDER_HPP
#define BOOST_DI_DETAIL_BINDER_HPP

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
#include "boost/di/aux/utility.hpp"

namespace boost {
namespace di {
namespace detail {

namespace detail {

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

template<typename TCallStack, typename TContext>
struct equal_call_stack
    : mpl::equal<
        mpl::iterator_range<
            typename mpl::advance_c<
                typename mpl::begin<TCallStack>::type
              , mpl::size<TCallStack>::value - mpl::size<TContext>::value
            >::type
          , typename mpl::end<TCallStack>::type
        >
      , TContext
      >
{ };

template<typename T1, typename T2>
struct less_context_size
    : mpl::bool_<(
          aux::get_context_size<T1>::value
          >
          aux::get_context_size<T2>::value
      )>::type
{ };

template<typename T, typename TBind>
struct comparator
    : mpl::apply<TBind, T>::type
{ };

template<
    typename T
  , typename TCallStack
  , typename TDeps
> struct sort_dependecies_by_call_stack_order
    : mpl::sort<
        typename mpl::fold<
            TDeps
          , mpl::vector0<>
          , mpl::if_<
                mpl::and_<
                    detail::comparator<
                        typename aux::make_plain<T>::type
                      , aux::get_bind<mpl::_2>
                    >
                  , detail::equal_call_stack<
                        TCallStack
                      , aux::get_context<mpl::_2>
                    >
                >
              , mpl::push_back<mpl::_1, mpl::_2>
              , mpl::_1
            >
        >::type
      , detail::less_context_size<mpl::_1, mpl::_2>
    >::type
{ };

template<
    typename T
  , typename TDefault
  , typename = void
>
struct make_default_dependency
    : TDefault::template rebind<
          typename aux::make_plain<T>::type
        , typename aux::make_plain<T>::type
      >
{ };

template<
    typename T
  , typename TDefault
>
struct make_default_dependency<
    T
  , TDefault
  , typename enable_if<has_element_type<T> >::type
>
    : TDefault::template rebind<
          typename aux::make_plain<T>::type
        , typename aux::make_plain<typename T::value_type>::type
      >
{ };

} // namespace detail

template<
    typename T
  , typename TCallStack
  , typename TDeps
  , typename TDefault
>
struct binder
    : mpl::deref<
          mpl::begin<
              typename mpl::push_back<
                  typename detail::sort_dependecies_by_call_stack_order<
                      T
                    , TCallStack
                    , TDeps
                  >::type
                , typename detail::make_default_dependency<
                      T
                    , TDefault
                  >::type
              >::type
          >
      >::type
{ };

} // namespace detail
} // namespace di
} // namespace boost

#endif

