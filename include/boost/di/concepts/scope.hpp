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
#include <boost/mpl/push_back.hpp>
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

    template<
        typename T
      , typename U
      , typename = void
    >
    struct rebind
        : T::template rebind<U>::other
    { };

    template<
        typename T
      , BOOST_DI_TYPES_DEFAULT_MPL(T)
    >
    struct get_dependencies
        : mpl::fold<
              mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
            , mpl::vector0<>
            , mpl::push_back<
                  mpl::_1
                , mpl::if_<
                      is_dependency<mpl::_2>
                    , rebind<mpl::_2, TScope>
                    , rebind<T, TScope>
                  >
              >
        >::type
    { };

public:
    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct bind
        : get_dependencies<
              dependency<mpl::_2>
            , BOOST_DI_TYPES_PASS_MPL(T)
          >
    { };

    template<typename TExpected>
    struct bind<TExpected, BOOST_DI_TYPES_MPL_NA(1)>
        : get_dependencies<
              dependency<mpl::_2>
            , TExpected
          >
    {
        template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
        struct in_call
            : get_dependencies<
                  dependency<
                      mpl::_2
                    , mpl::_2
                    , void
                    , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                  >
                , TExpected
              >
        {
            template<typename TName>
            struct in_name
                : get_dependencies<
                      dependency<
                          mpl::_2
                        , mpl::_2
                        , TName
                        , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                      >
                    , TExpected
                  >
            { };
        };

        template<typename TName>
        struct in_name
            : get_dependencies<
                  dependency<
                      mpl::_2
                    , mpl::_2
                    , TName
                  >
                , TExpected
              >
        {
            template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
            struct in_call
                : get_dependencies<
                      dependency<
                          mpl::_2
                        , mpl::_2
                        , TName
                        , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                      >
                    , TExpected
                  >
            { };
        };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

