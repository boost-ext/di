//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_SCOPE_HPP
#define BOOST_DI_CONCEPTS_SCOPE_HPP

#include "boost/di/aux_/meta.hpp"
#include "boost/di/type_traits/is_same_base_of.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/has_xxx.hpp>

#include <boost/di/scopes/unique.hpp>

namespace boost {
namespace di {
namespace concepts {

template<
    typename TScope
  , template<
        typename
      , typename T
      , typename = T
      , typename TName = void
      , typename = ::boost::mpl::vector0<>
      , typename = typename ::boost::mpl::lambda<
            ::boost::mpl::and_<
                ::boost::di::type_traits::is_same_base_of<T, ::boost::mpl::_1>
              , ::boost::is_same<TName, ::boost::mpl::_2>
            >
        >::type
    > class TDependency
>
class scope
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(context)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)

    template<
        typename TExpected
      , typename TGiven = TExpected
      , typename TName = void
      , typename TContext = mpl::vector0<>
    >
    struct dependency
        : TDependency<
               mpl::_1
             , TExpected
             , TGiven
             , TName
             , TContext
          >
    { };

    template<typename T>
    struct is_dependency
        : has_context<T>
    { };

    template<typename T>
    struct deps
    {
        typedef typename T::deps type;
    };

    template<typename T>
    struct scope_
    {
        typedef typename T::scope type;
    };

    template<typename T>
    struct name
    {
        typedef typename T::name type;
    };

    template<typename T>
    struct call_stack
    {
        typedef typename T::call_stack type;
    };

    template<typename T, typename U>
    struct rebind
        : T::template rebind<U>::other
    { };

    template<
        typename TSeq
      , typename TName = void
      , typename TCallStack = mpl::vector0<>
      , typename T = TScope
    >
    struct get_deps
        : mpl::fold<
              TSeq
            , mpl::vector0<>
            , mpl::copy<
                  mpl::if_<
                      is_dependency<mpl::_2>
                    , mpl::vector1<rebind<mpl::_2, T> >
                    , mpl::if_<
                          has_deps<mpl::_2>
                        , get_deps<
                              deps<mpl::_2>
                            , name<mpl::_2>
                            , call_stack<mpl::_2>
                            , rebind<TScope, scope_<mpl::_2> >
                          >
                        , mpl::vector1<
                              rebind<
                                  dependency<mpl::_2, mpl::_2, TName, TCallStack>
                                , T
                              >
                          >
                      >
                  >
                , mpl::back_inserter<mpl::_1>
              >
          >::type
    {
        typedef TSeq deps;
        typedef TName name;
        typedef TCallStack call_stack;
        typedef TScope scope;
    };

public:
    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct bind
        : get_deps<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
    { };

    template<typename TExpected>
    struct bind<TExpected, BOOST_DI_TYPES_MPL_NA(1)>
        : get_deps<mpl::vector<TExpected> >
    {
        template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
        struct in_call
            : get_deps<
                  mpl::vector<TExpected>
                , void
                , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
              >
        {
            template<typename TName>
            struct in_name
                : get_deps<
                      mpl::vector<TExpected>
                    , TName
                    , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                  >
            { };
        };

        template<typename TName>
        struct in_name
            : get_deps<
                  mpl::vector<TExpected>
                , TName
              >
        {
            template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
            struct in_call
                : get_deps<
                      mpl::vector<TExpected>
                    , TName
                    , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                  >
            { };
        };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

