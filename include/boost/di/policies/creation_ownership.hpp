//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CREATION_OWNERSHIP_HPP
#define BOOST_DI_POLICIES_CREATION_OWNERSHIP_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>

namespace boost {
namespace di {
namespace policies {

/**
 * @code
 * injector_.create<const c1&>();
 * injector_.create<c1&>();
 * @endcode
 */
class creation_ownership
{
public:
    typedef creation_ownership is_policy;

    template<
        typename TDeps
      , typename TGiven
      , typename TAssert = mpl::true_
      , typename = void
    >
    struct verify
        : mpl::true_
    { };

    template<
        typename TDeps
      , typename TGiven
      , typename TAssert
    >
    struct verify<
        TDeps
      , TGiven
      , TAssert
      , typename enable_if<is_reference<TGiven> >::type
    >
        : mpl::false_
    {
       BOOST_MPL_ASSERT_MSG(
            !TAssert::value
          , CREATION_OWNERSHIP_IS_NOT_CLEAR
          , (TGiven)
        );
    };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

