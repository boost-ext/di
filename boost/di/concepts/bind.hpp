//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_BIND_HPP
#define BOOST_DI_CONCEPTS_BIND_HPP

#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include "boost/di/aux/dependency.hpp"
#include "boost/di/aux/explicit_value.hpp"
#include "boost/di/aux/utility.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace concepts {

template<
    typename TExpected
  , typename TGiven = TExpected
  , typename Enable = void
>
struct bind
    : aux::dependency<
          mpl::_1
        , TExpected
        , TGiven
        , mpl::vector0<>
      >
    , aux::internal
{
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct in_call_stack
        : aux::dependency<
              mpl::_1
            , TExpected
            , TGiven
            , mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
          >
        , aux::internal
    {
        template<typename TName>
        struct in_name
            : aux::dependency<
                  mpl::_1
                , named<TExpected, TName>
                , TGiven
                , mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
              >
            , aux::internal
        { };
    };

    template<typename TName>
    struct in_name
        : aux::dependency<
              mpl::_1
            ,  named<TExpected, TName>
            ,  TGiven
          >
        , aux::internal
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct in_call_stack
            : aux::dependency<
                  mpl::_1
                , named<TExpected, TName>
                , TGiven
                , mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
              >
            , aux::internal
        { };
    };
};

template<typename TExpected, typename TGiven>
struct bind<
    TExpected
  , TGiven
  , typename enable_if<
        mpl::or_<
            aux::explicit_value<TGiven>
          , is_same<TExpected, TGiven>
        >
    >::type
>
    : aux::dependency<
          mpl::_1
        , TExpected
        , TGiven
        , mpl::vector0<>
        , mpl::or_<
              is_base_of<mpl::_1, TExpected>
            , is_same<mpl::_1, TExpected>
          >
      >
    , aux::internal
{
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct in_call_stack
        : aux::dependency<
              mpl::_1
            , TExpected
            , TGiven
            , mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
            , mpl::or_<
                  is_base_of<mpl::_1, TExpected>
                , is_same<mpl::_1, TExpected>
              >
          >
        , aux::internal
    {
        template<typename TName>
        struct in_name
            : aux::dependency<
                  mpl::_1
                , named<TExpected, TName>
                , TGiven
                , mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
                , mpl::or_<
                      is_base_of<mpl::_1, named<TExpected, TName> >
                    , is_same<mpl::_1, named<TExpected, TName> >
                  >
              >
            , aux::internal
        { };
    };

    template<typename TName>
    struct in_name
        : aux::dependency<
              mpl::_1
            , named<TExpected, TName>
            , TGiven
            , mpl::vector0<>
            , mpl::or_<
                  is_base_of<mpl::_1, named<TExpected, TName> >,
                  is_same<mpl::_1, named<TExpected, TName> >
              >
          >
        , aux::internal
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct in_call_stack
            : aux::dependency<
                  mpl::_1
                , named<TExpected, TName>
                , TGiven
                , mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
                , mpl::or_<
                      is_base_of<mpl::_1, named<TExpected, TName> >,
                      is_same<mpl::_1, named<TExpected, TName> >
                  >
              >
            , aux::internal
        { };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

