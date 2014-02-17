//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if defined(BOOST_DI_PREPROCESSED)
    #include "boost/di/aux_/preprocessed/concepts/bind.hpp"
#endif

#ifndef BOOST_DI_CONCEPTS_BIND_HPP
#define BOOST_DI_CONCEPTS_BIND_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/concepts/type_traits/is_req_type.hpp"
#include "boost/di/concepts/type_traits/is_req_name.hpp"
#include "boost/di/concepts/type_traits/is_req_call.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/times.hpp>

namespace boost {
namespace di {
namespace concepts {

template<
    typename TExpected
  , typename TGiven
  , template<
        typename
      , typename
      , typename
      , typename
    > class TDependency
>
struct bind
    : TDependency<
          mpl::_1
        , TExpected
        , TGiven
        , typename mpl::lambda<
              type_traits::is_req_type<TExpected>
          >::type
      >
{
    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct when
        : TDependency<
              mpl::_1
            , TExpected
            , TGiven
            , typename mpl::lambda<
                  mpl::times<
                      type_traits::is_req_type<TExpected>
                    , type_traits::is_req_call<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
                  >
              >::type
          >
    {
        template<typename TName>
        struct named
            : TDependency<
                  mpl::_1
                , TExpected
                , TGiven
                , typename mpl::lambda<
                      mpl::times<
                          mpl::times<
                              type_traits::is_req_type<TExpected>
                            , type_traits::is_req_name<TName>
                          >
                        , type_traits::is_req_call<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
                      >
                >::type
              >
        { };
    };

    template<typename TName>
    struct named
        : TDependency<
              mpl::_1
            , TExpected
            , TGiven
            , typename mpl::lambda<
                  mpl::times<
                      type_traits::is_req_type<TExpected>
                    , type_traits::is_req_name<TName>
                  >
            >::type
          >
    {
        template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
        struct when
            : TDependency<
                  mpl::_1
                , TExpected
                , TGiven
                , typename mpl::lambda<
                      mpl::times<
                          mpl::times<
                              type_traits::is_req_type<TExpected>
                            , type_traits::is_req_name<TName>
                          >
                        , type_traits::is_req_call<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
                      >
                >::type
              >
        { };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

