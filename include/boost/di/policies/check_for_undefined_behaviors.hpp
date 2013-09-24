//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CHECK_FOR_UNDEFINED_BEHAVIORS_HPP
#define BOOST_DI_POLICIES_CHECK_FOR_UNDEFINED_BEHAVIORS_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/assert.hpp>

#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/detail/binder.hpp"

namespace boost {
namespace di {
namespace policies {

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
          //mpl::filter_view<TDeps, context<mpl::_> >
            typename mpl::fold<
                TDeps
              , mpl::vector0<>
              , mpl::push_back<mpl::_1, context<mpl::_2> >
            >::type
        , T
      >
{ };


template<typename TDeps>
struct is_undefined_behavior
    : mpl::count_if<
          typename get_singletons<TDeps>::type
        , is_in_any_context<TDeps, mpl::_>
      >
{ };

/**
 * @code
 * struct s { s(int); };
 * struct k { k(shared_ptr<s>); };
 * struct c { c(shared_ptr<s>, k); }; //evolution order is not specified
 * injector<
 *     bind<int>::in_call<s>::to(i)
 *   , singleton<s>
 * >().create<c>();
 * @endcode
 */
class check_for_undefined_behaviors
{
public:
    template<
        typename TDeps
      , typename TGiven
      , bool Assert = true
      , template<typename> class TBinder = detail::binder
    >
    class verify
    {
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
              >::type
        { };

/*        template<*/
            //typename T
          //, typename TBind
          //, typename TCallStack
        //>
        //struct deps<
            //T
          //, TBind
          //, TCallStack
          //, typename disable_if<is_undefined_behavior<TCallStack> >::type
        //>
            //: mpl::false_
        //{
            ////BOOST_MPL_ASSERT_MSG(
                ////!Assert
              ////, CIRCULAR_DEPENDENCIES_ARE_NOT_ALLOWED
              ////, (T, TCallStack)
            ////);
        //};

    public:
/*        typedef typename is_undefined_behavior<*/
            //typename deps<TGiven, TBinder<TDeps> >::type
        /*>::type s;*/

        template<typename Q>
        struct for_each_context
            : mpl::fold<
                  typename mpl::transform<Q, context<mpl::_> >::type
                , mpl::vector0<>
                , mpl::push_back<
                    mpl::_1
                  , typename mpl::fold<
                          mpl::_2
                        , mpl::_1
                        , mpl::push_back<mpl::_1, get_type<mpl::_2> >
                    >::type
                  >
              >
        { };

        typedef typename for_each_context<typename deps<TGiven, TBinder<TDeps> >::type>::type s;
    };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

