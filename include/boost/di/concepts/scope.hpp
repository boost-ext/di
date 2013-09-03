//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_SCOPE_HPP
#define BOOST_DI_CONCEPTS_SCOPE_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/push_back.hpp>

#include "boost/di/type_traits/is_same_base_of.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace concepts {

template<
    typename TScope
  , template<
        typename
      , typename T
      , typename = T
      , typename = mpl::vector0<>
      , typename = type_traits::is_same_base_of<T>
    > class TDependency
  , template<
        typename
      , typename
      , typename = void
    > class TNamed
>
class scope
{
    template<
        typename T
      , typename U
    >
    struct rebind
        : T::template rebind<U>::other
    { };

    template<typename T>
    struct is_dependency
        : type_traits::has_context<T>
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
    template<
        typename TExpected
      , typename TGiven
      , typename TContext = mpl::vector0<>
    >
    struct dependency_impl
        : dependency<
               mpl::_1
             , TExpected
             , TGiven
             , TContext
             , type_traits::is_same_base_of<TExpected>
          >
    { };

    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct bind
        : get_dependencies<
              //TDependency<TScope, mpl::_2>
              dependency_impl<mpl::_2, mpl::_2>
            , BOOST_DI_TYPES_PASS_MPL(T)
          >
    { };

    template<typename TExpected>
    struct bind<TExpected, BOOST_DI_TYPES_MPL_NA(1)>
        : get_dependencies<
              dependency_impl<mpl::_2, mpl::_2>
              //TDependency<TScope, mpl::_2>
            , TExpected
          >
    {
        template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
        struct in_call
            : get_dependencies<
                  dependency_impl<mpl::_2, mpl::_2, mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
                  //TDependency<
                      //TScope
                    //, mpl::_2
                    //, mpl::_2
                    //, mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                  //>
                , TExpected
              >
        {
            template<typename TName>
            struct in_name
                : get_dependencies<
                      dependency_impl<TNamed<mpl::_2, TName>, mpl::_2, mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
                      //TDependency<
                          //TScope
                        //, TNamed<mpl::_2, TName>
                        //, mpl::_2
                        //, mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                      //>
                    , TExpected
                  >
            { };
        };

        template<typename TName>
        struct in_name
            : get_dependencies<
                  dependency_impl<TNamed<mpl::_2, TName>, mpl::_2>
                  //TDependency<TScope, TNamed<mpl::_2, TName>, mpl::_2>
                , TExpected
              >
        {
            template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
            struct in_call
                : get_dependencies<
                        dependency_impl<TNamed<mpl::_2, TName>, mpl::_2, mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>>
                      //TDependency<
                          //TScope
                        //, TNamed<mpl::_2, TName>
                        //, mpl::_2
                        //, mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                      //>
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

