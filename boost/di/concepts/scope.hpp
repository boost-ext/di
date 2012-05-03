//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_SCOPE_HPP
#define BOOST_DI_CONCEPTS_SCOPE_HPP

#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/push_back.hpp>
#include "boost/di/concepts/bind.hpp"
#include "boost/di/aux/dependency.hpp"
#include "boost/di/concepts/annotate.hpp"
#include "boost/di/named.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace concepts {

template<typename TScope>
class scope
{
    template<typename TRebind, typename T>
    struct rebind
        : TRebind::template rebind<T>
    { };

public:
    template<
        typename TExpected
      , typename TGiven
      , typename TContext = mpl::vector0<>
    >
    struct dependency
        : aux::dependency<
              mpl::_1
             , TExpected
             , TGiven
             , TContext
             , mpl::or_<
                  is_base_of<mpl::_1, TExpected>,
                  is_same<mpl::_1, TExpected>
               >
          >
        , annotate<>::with<>
    { };

    template<BOOST_DI_ARGS_TYPES_MPL(T)>
    struct bind
        : mpl::fold<
              mpl::vector<BOOST_DI_ARGS_MPL(T)>
            , mpl::vector0<>
            , mpl::if_<
                  is_base_of<annotate<>::with<>, mpl::_2>
                , mpl::push_back<
                      mpl::_1
                    , rebind<mpl::_2, TScope>
                  >
                , mpl::push_back<
                      mpl::_1
                    , rebind<dependency<mpl::_2, mpl::_2>, TScope>
                  >
              >
          >::type
        , annotate<>::with<>
    { };

    template<typename TExpected>
    struct bind<TExpected, BOOST_DI_ARGS_MPL_NA(1)>
        : mpl::fold<
              mpl::vector1<TExpected>
            , mpl::vector0<>
            , mpl::if_<
                  is_base_of<annotate<>::with<>, mpl::_2>
                , mpl::push_back<
                      mpl::_1
                    , rebind<mpl::_2, TScope>
                  >
                , mpl::push_back<
                      mpl::_1
                    , rebind< dependency<mpl::_2, mpl::_2>, TScope>
                  >
              >
          >::type
        , annotate<>::with<>
    {
        template<BOOST_DI_ARGS_TYPES_MPL(T)>
        struct in_call
            : mpl::fold<
                  mpl::vector1<TExpected>
                , mpl::vector0<>
                , mpl::if_<
                      is_base_of<annotate<>::with<>, mpl::_2>
                    , mpl::push_back<
                          mpl::_1
                        , rebind<mpl::_2, TScope>
                      >
                    , mpl::push_back<
                          mpl::_1
                        , rebind<
                              dependency<
                                  mpl::_2
                                , mpl::_2
                               , mpl::vector<BOOST_DI_ARGS_MPL(T)>
                              >
                            , TScope
                          >
                      >
                  >
              >::type
            , annotate<>::with<>
        {
            template<typename TName>
            struct in_name
                : mpl::fold<
                      mpl::vector1<TExpected>
                    , mpl::vector0<>
                    , mpl::if_<
                          is_base_of<annotate<>::with<>, mpl::_2>
                        , mpl::push_back<
                              mpl::_1
                            , rebind<mpl::_2, TScope>
                          >
                        , mpl::push_back<
                              mpl::_1
                            , rebind<
                                  dependency<
                                      named<mpl::_2, TName>
                                    , mpl::_2, mpl::vector<BOOST_DI_ARGS_MPL(T)>
                                  >
                                , TScope
                              >
                          >
                      >
                  >::type
                , annotate<>::with<>
            { };
        };

        template<typename TName>
        struct in_name
            : mpl::fold<
                  mpl::vector1<TExpected>
                , mpl::vector0<>
                , mpl::if_<
                      is_base_of<annotate<>::with<>, mpl::_2>
                    , mpl::push_back<
                          mpl::_1
                        , rebind<mpl::_2, TScope>
                      >
                    , mpl::push_back<
                          mpl::_1
                        , rebind<
                              dependency<
                                  named<mpl::_2, TName>
                                , mpl::_2
                              >
                            , TScope
                          >
                      >
                  >
              >::type
            , annotate<>::with<>
        {
            template<BOOST_DI_ARGS_TYPES_MPL(T)>
            struct in_call
                : mpl::fold<
                    mpl::vector1<TExpected>
                  , mpl::vector0<>
                  , mpl::if_<
                        is_base_of<annotate<>::with<>, mpl::_2>
                      , mpl::push_back<
                            mpl::_1
                          , rebind<mpl::_2, TScope>
                        >
                      , mpl::push_back<
                            mpl::_1
                          , rebind<
                                dependency<
                                    named<mpl::_2, TName>
                                  , mpl::_2
                                  , mpl::vector<BOOST_DI_ARGS_MPL(T)>
                                >
                              , TScope
                            >
                        >
                    >
                  >::type
                , annotate<>::with<>
            { };
        };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

