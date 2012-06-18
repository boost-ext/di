//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CHECK_FOR_CREATION_OWNERSHIP_HPP
#define BOOST_DI_POLICIES_CHECK_FOR_CREATION_OWNERSHIP_HPP

#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/mpl/assert.hpp>

#include "boost/di/aux_/meta_config.hpp"

namespace boost {
namespace di {
namespace policies {

class check_for_creation_ownership
{
public:
    template<
        typename TDeps
      , typename TExternals
      , typename TGiven
      , bool Assert = true
      , typename = void
    >
    class verify
        : public mpl::true_
    { };

    template<
        typename TDeps
      , typename TExternals
      , typename TGiven
      , bool Assert
    >
    class verify<
        TDeps
      , TExternals
      , TGiven
      , Assert
      , typename enable_if<is_reference<TGiven> >::type
    >
        : public mpl::false_
    {
       BOOST_MPL_ASSERT_MSG(
            !Assert
          , CREATION_OWNERSHIP_IS_NOT_CLEAR
          , (TGiven)
        );
    };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

