#ifndef BOOST_DI_POLICIES_BINDING_CORRECTNESS_HPP
#define BOOST_DI_POLICIES_BINDING_CORRECTNESS_HPP

#include "boost/di_preprocessed/detail/binder.hpp"
#include "boost/di_preprocessed/type_traits/ctor_traits.hpp"
#include "boost/di_preprocessed/type_traits/make_plain.hpp"
#include "boost/di_preprocessed/scopes/shared.hpp"
#include "boost/di_preprocessed/scopes/session.hpp"
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/assert.hpp>

namespace boost {
namespace di {
namespace policies {
class binding_correctness
{
    template<typename>
    struct is_scope
        : mpl::false_
    { };

    template<template<typename> class TConvertible>
    struct is_scope<scopes::shared<TConvertible> >
        : mpl::true_
    { };

    template<template<typename> class TConvertible>
    struct is_scope<scopes::session<TConvertible> >
        : mpl::true_
    { };

    template<typename T>
    struct scope_
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
    struct deps_
        : mpl::fold<
              ctor<typename binder<T, TCallStack, TBind>::given>
            , mpl::vector0<>
            , mpl::copy<
                  mpl::joint_view<
                      mpl::vector1<binder<mpl::_2, TCallStack, TBind> >
                    , deps_<
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
                  is_scope<scope_<mpl::_2> >
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
    struct undefined_behavior_impl
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

    template<typename TBind, typename T, typename TName>
    struct comparator
        : mpl::apply<TBind, T, TName>::type
    { };

    template<
        typename TCtor
      , typename TDependency
    >
    struct ctor_has_type
        : mpl::count_if<
              TCtor
            , comparator<
                  typename TDependency::bind
                , mpl::_1
                , typename TDependency::name
              >
          >::type
    { };

    template<typename T>
    struct get_context
        : mpl::fold<
              typename T::context
            , mpl::vector0<>
            , mpl::if_<
                  mpl::is_sequence<mpl::_2>
                , mpl::push_back<
                      mpl::_1
                    , mpl::back<mpl::_2>
                  >
                , mpl::push_back<mpl::_1, mpl::_2>
              >
          >::type
    { };

    template<typename TDependency, typename TResult>
    struct for_each_context
        : mpl::fold<
              get_context<TDependency>
            , TResult
            , mpl::if_<
                 ctor_has_type<
                      ctor<mpl::_2>
                    , TDependency
                  >
                , mpl::_1
                , mpl::push_back<
                      mpl::_1
                    , mpl::pair<
                          typename expected<TDependency>::type
                        , mpl::_2
                      >
                  >
              >
          >::type
    { };

    template<typename TDeps>
    struct binding_impl
        : mpl::fold<
              TDeps
            , mpl::vector0<>
            , for_each_context<mpl::_2, mpl::_1>
          >::type
    { };

    template<
        typename TDeps
      , typename TAssert
      , typename T = typename binding_impl<TDeps>::type
    >
    struct binding : T
    {
       BOOST_MPL_ASSERT_MSG(
            !TAssert::value || mpl::empty<T>::value
          , BINDING_DECLARATION_IS_NOT_CORRECT
          , (T)
        );
    };

    template<
        typename TDeps
      , typename TGiven
      , typename TBinder
      , typename TAssert
      , typename T = typename undefined_behavior_impl<
            typename deps_<TGiven, TBinder>::type
        >::type
    >
    struct undefined_behavior : T
    {
        BOOST_MPL_ASSERT_MSG(
            !TAssert::value || mpl::empty<T>::value
          , ARGUMENTS_EVALUTION_ORDER_UNDEFINED_BEHAVIOR
          , (T)
        );
    };

public:
    template<
        typename TDeps
      , typename TGiven
      , typename TAssert = mpl::true_
      , template<typename> class TBinder = di::detail::binder
    >
    struct verify
        : mpl::joint_view<
              binding<TDeps, TAssert>
            , undefined_behavior<TDeps, TGiven, TBinder<TDeps>, TAssert>
          >
    { };

    template<
        typename TDeps
      , typename T
    >
    static void assert_policy() { }
};

}
}
}

#endif

