//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_BIND_HPP
#define BOOST_DI_CONCEPTS_BIND_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include "boost/di/aux/dependency.hpp"
#include "boost/di/aux/explicit_value.hpp"
#include "boost/di/concepts/annotate.hpp"
#include "boost/di/named.hpp"
#include "boost/di/config.hpp"

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
    , annotate<>::with<>
{
    template<BOOST_DI_ARGS_TYPES_MPL(T)>
    struct in_call
        : aux::dependency<
              mpl::_1
            , TExpected
            , TGiven
            , mpl::vector<BOOST_DI_ARGS_MPL(T)>
          >
        , annotate<>::with<>
    {
        template<typename TName>
        struct in_name
            : aux::dependency<
                  mpl::_1
                , named<TExpected, TName>
                , TGiven
                , mpl::vector<BOOST_DI_ARGS_MPL(T)>
              >
            , annotate<>::with<>
        { };
    };

    template<typename TName>
    struct in_name
        : aux::dependency<
              mpl::_1
            ,  named<TExpected, TName>
            ,  TGiven
          >
        , annotate<>::with<>
    {
        template<BOOST_DI_ARGS_TYPES_MPL(T)>
        struct in_call
            : aux::dependency<
                  mpl::_1
                , named<TExpected, TName>
                , TGiven
                , mpl::vector<BOOST_DI_ARGS_MPL(T)>
              >
            , annotate<>::with<>
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
    , annotate<>::with<>
{
    template<BOOST_DI_ARGS_TYPES_MPL(T)>
    struct in_call
        : aux::dependency<
              mpl::_1
            , TExpected
            , TGiven
            , mpl::vector<BOOST_DI_ARGS_MPL(T)>
            , mpl::or_<
                  is_base_of<mpl::_1, TExpected>
                , is_same<mpl::_1, TExpected>
              >
          >
        , annotate<>::with<>
    {
        template<typename TName>
        struct in_name
            : aux::dependency<
                  mpl::_1
                , named<TExpected, TName>
                , TGiven
                , mpl::vector<BOOST_DI_ARGS_MPL(T)>
                , mpl::or_<
                      is_base_of<mpl::_1, named<TExpected, TName> >
                    , is_same<mpl::_1, named<TExpected, TName> >
                  >
              >
            , annotate<>::with<>
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
        , annotate<>::with<>
    {
        template<BOOST_DI_ARGS_TYPES_MPL(T)>
        struct in_call
            : aux::dependency<
                  mpl::_1
                , named<TExpected, TName>
                , TGiven
                , mpl::vector<BOOST_DI_ARGS_MPL(T)>
                , mpl::or_<
                      is_base_of<mpl::_1, named<TExpected, TName> >,
                      is_same<mpl::_1, named<TExpected, TName> >
                  >
              >
            , annotate<>::with<>
        { };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

