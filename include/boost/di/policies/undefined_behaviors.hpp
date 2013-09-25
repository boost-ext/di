//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_UNDEFINED_BEHAVIORS_HPP
#define BOOST_DI_POLICIES_UNDEFINED_BEHAVIORS_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/assert.hpp>

#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/detail/binder.hpp"

namespace boost {
namespace di {
namespace policies {

/**
 * @code
 * struct s { s(int); };
 * struct k { k(shared_ptr<s>); };
 * struct c { c(shared_ptr<s>, shared_ptr<k>); }; //arguments evaluation order is not specified
 * injector<
 *     bind<int>::in_call<s>::to(i)
 *   , singleton<s>
 * >().create<c>();
 * @endcode
 */
class undefined_behaviors
{
    template<typename>
    struct is_singleton
        : mpl::false_
    { };

    template<template<typename> class TConvertible>
    struct is_singleton<scopes::singleton<TConvertible> >
        : mpl::true_
    { };

    template<typename T>
    struct scope
    {
        typedef typename T::scope type;
    };

    template<typename T>
    struct expected
    {
        typedef typename T::expected type;
    };

    template<typename T>
    struct context
    {
        typedef typename T::context type;
    };

    template<typename T, typename = void>
    struct get_type
    {
        typedef T type;
    };

    template<typename T>
    struct get_type<T, typename enable_if<mpl::is_sequence<T> >::type>
    {
        typedef typename mpl::back<T>::type type;
    };

    template<typename T>
    struct ctor
        : type_traits::ctor_traits<
              typename type_traits::make_plain<T>::type
          >::type
    { };

    template<
        typename T
      , typename TCallStack
      , typename TBind
    >
    struct binder
        : TBind::template get_dependency<T, TCallStack>::type
    { };

    template<
        typename T
      , typename TBind
      , typename TCallStack =
            mpl::vector1<typename type_traits::make_plain<T>::type>
    >
    struct deps
        : mpl::fold<
              ctor<typename binder<T, TCallStack, TBind>::given>
            , mpl::vector0<>
            , mpl::copy<
                  mpl::joint_view<
                      mpl::vector1<binder<mpl::_2, TCallStack, TBind> >
                    , deps<
                          mpl::_2
                        , TBind
                        , mpl::push_back<
                              TCallStack
                            , type_traits::make_plain<mpl::_2>
                          >
                      >
                  >
                , mpl::back_inserter<mpl::_1>
              >
          >
    { };

    template<typename TDeps>
    struct get_singletons
        : mpl::fold<
              TDeps
            , mpl::vector0<>
            , mpl::if_<
                  is_singleton<scope<mpl::_2> >
                , mpl::push_back<mpl::_1, expected<mpl::_2> >
                , mpl::_1
              >
          >
    { };

    template<typename TDeps, typename T>
    struct is_in_any_context
        : mpl::contains<
              typename mpl::fold<
                  typename mpl::fold<
                      typename mpl::transform<TDeps, context<mpl::_> >::type
                    , mpl::vector0<>
                    , mpl::copy<
                          mpl::_2
                        , mpl::back_inserter<mpl::_1>
                      >
                  >::type
                , mpl::vector0<>
                , mpl::push_back<mpl::_1, get_type<mpl::_2> >
              >::type
            , T
          >
    { };

    template<typename TDeps>
    struct undefined_behavior
        : mpl::fold<
              typename mpl::fold<
                  typename get_singletons<TDeps>::type
                , mpl::set0<>
                , mpl::if_<
                      is_in_any_context<TDeps, mpl::_2>
                    , mpl::insert<mpl::_1, mpl::_2>
                    , mpl::_1
                  >
              >::type
            , mpl::vector0<>
            , mpl::push_back<mpl::_1, mpl::_2>
          >
    { };

    template<typename T, bool Assert>
    class verify_impl
    {
        BOOST_MPL_ASSERT_MSG(
            !Assert || mpl::empty<T>::value
          , ARGUMENTS_EVALUTION_ORDER_UNDEFINED_BEHAVIOR
          , (T)
        );

    public:
        typedef T type;
    };

public:
    typedef undefined_behaviors is_policy;

    template<
        typename TDeps
      , typename TGiven
      , bool Assert = true
      , template<typename> class TBinder = detail::binder
    >
    struct verify
        : verify_impl<
              typename undefined_behavior<
                  typename deps<TGiven, TBinder<TDeps> >::type
              >::type
            , Assert
          >
    { };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

