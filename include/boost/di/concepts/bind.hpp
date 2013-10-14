//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_BIND_HPP
#define BOOST_DI_CONCEPTS_BIND_HPP

#include "boost/di/aux_/meta.hpp"
#include "boost/di/type_traits/is_same_base_of.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/lambda.hpp>

namespace boost {
namespace di {
namespace concepts {

template<
    typename TExpected
  , typename TGiven
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
struct bind
    : TDependency<
          mpl::_1
        , TExpected
        , TGiven
      >
{
    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct in_call
        : TDependency<
              mpl::_1
            , TExpected
            , TGiven
            , void
            , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
          >
    {
        template<typename TName>
        struct in_name
            : TDependency<
                  mpl::_1
                , TExpected
                , TGiven
                , TName
                , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
              >
        { };
    };

    template<typename TName>
    struct in_name
        : TDependency<
              mpl::_1
            , TExpected
            , TGiven
            , TName
          >
    {
        template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
        struct in_call
            : TDependency<
                  mpl::_1
                , TExpected
                , TGiven
                , TName
                , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
              >
        { };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

