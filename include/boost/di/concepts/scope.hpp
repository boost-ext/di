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
    BOOST_MPL_HAS_XXX_TRAIT_DEF(dependencies)

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
    struct dependencies
    {
        typedef typename T::dependencies type;
    };

    template<typename T, typename U>
    struct rebind
        : T::template rebind<U>::other
    { };

    template<
        typename TSeq
      , typename TName = void
      , typename TCallStack = mpl::vector0<>
    >
    struct get_dependencies
        : mpl::fold<
              TSeq
            , mpl::vector0<>
            , mpl::copy<
                  mpl::if_<
                      is_dependency<mpl::_2>
                    , mpl::vector1<rebind<mpl::_2, TScope> >
                    , mpl::if_<
                          has_dependencies<mpl::_2>
                        , get_dependencies<
                              dependencies<mpl::_2>
                            , TName
                            , TCallStack
                          >
                        , mpl::vector1<
                              rebind<
                                  dependency<mpl::_2, mpl::_2, TName, TCallStack>
                                , TScope
                              >
                          >
                      >
                  >
                , mpl::back_inserter<mpl::_1>
              >
        >::type
    {
        typedef TSeq dependencies;
    };

public:
    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct bind
        : get_dependencies<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
    { };

    template<typename TExpected>
    struct bind<TExpected, BOOST_DI_TYPES_MPL_NA(1)>
        : get_dependencies<mpl::vector1<TExpected> >
    {
        template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
        struct in_call
            : get_dependencies<
                  mpl::vector1<TExpected>
                , void
                , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
              >
        {
            template<typename TName>
            struct in_name
                : get_dependencies<
                      mpl::vector1<TExpected>
                    , TName
                    , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                  >
            { };
        };

        template<typename TName>
        struct in_name
            : get_dependencies<
                  mpl::vector1<TExpected>
                , TName
              >
        {
            template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
            struct in_call
                : get_dependencies<
                      mpl::vector1<TExpected>
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

