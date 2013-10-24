//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_BINDER_HPP
#define BOOST_DI_DETAIL_BINDER_HPP

#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/type_traits/scope_traits.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"
#include "boost/di/concepts/dependency.hpp"

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
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/min_max.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace di {
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
                    , mpl::minus<
                          mpl::size<TCallStack>
                        , mpl::size<TContext>
                      >
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

template<typename TCallStack, typename TContext>
struct for_each_context
    : mpl::fold<
          TContext
        , mpl::int_<
              equal_call_stack<TCallStack, TContext>::value
          >
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

template<typename T>
struct priority
{
    typedef typename T::priority type;
};

template<typename T1, typename T2>
struct less_context_size
    : mpl::or_<
          mpl::less<
              get_longest_context_size<make_context<T2> >
            , get_longest_context_size<make_context<T1> >
          >
        , mpl::and_<
              mpl::equal<
                  get_longest_context_size<make_context<T2> >
                , get_longest_context_size<make_context<T1> >
              >
            , mpl::less<
                 typename priority<typename T2::scope>::type
               , typename priority<typename T1::scope>::type
              >
          >
      >
{ };

template<
    typename TCallStack
  , typename TDeps
  , typename TCond
>
struct sort_dependencies_by_call_stack_order
    : mpl::sort<
          typename mpl::fold<
              TDeps
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
  , typename TDeps
  , typename TDefault
  , typename TCond
>
struct get_dependency_by_call_stack_order
    : mpl::deref<
          mpl::begin<
              typename mpl::push_back<
                  typename sort_dependencies_by_call_stack_order<
                      TCallStack
                    , TDeps
                    , TCond
                  >::type
                , TDefault
              >::type
          >
      >::type
{ };

BOOST_MPL_HAS_XXX_TRAIT_DEF(name)
BOOST_MPL_HAS_XXX_TRAIT_DEF(named_type)

template<typename T, typename = void>
struct name
{
    typedef void type;
};

template<typename T>
struct name<T, typename enable_if<
    has_name<typename type_traits::remove_accessors<T>::type> >::type
>
{
    typedef typename type_traits::remove_accessors<T>::type::name type;
};

template<typename T, typename = void>
struct named
{
    typedef T type;
};

template<typename T>
struct named<T, typename enable_if<
    has_named_type<typename type_traits::remove_accessors<T>::type> >::type
>
{
    typedef typename type_traits::remove_accessors<T>::type type;
};

template<typename TBind, typename T, typename TName>
struct comparator
    : mpl::apply<TBind, T, TName>::type
{ };

template<
    typename T
  , typename TCallStack
  , typename TDeps
  , typename TDefault =
        ::boost::di::concepts::dependency<
            typename type_traits::scope_traits<typename named<T>::type>::type
          , typename type_traits::make_plain<T>::type
        >
>
struct binder_impl
    : get_dependency_by_call_stack_order<
          TCallStack
        , TDeps
        , TDefault
        , mpl::and_<
              comparator<
                  bind<mpl::_2>
                , typename type_traits::make_plain<T>::type
                , typename name<T>::type
              >
            , for_each_context<
                  TCallStack
                , make_context<mpl::_2>
              >
          >
      >::type::template rebind<
          typename type_traits::scope_traits<typename named<T>::type>::type
      >::other
{ };

template<typename TDeps = mpl::vector0<> >
struct binder
{
    template<typename T, typename TCallStack>
    struct get_dependency
        : binder_impl<T, TCallStack, TDeps>
    { };
};

} // namespace detail
} // namespace di
} // namespace boost

#endif

