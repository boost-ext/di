//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CREATION_OWNERSHIP_HPP
#define BOOST_DI_POLICIES_CREATION_OWNERSHIP_HPP

#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_reference.hpp>
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
    template<
        typename TDeps
      , typename TGiven
      , bool Assert = true
      , typename = void
    >
    class verify
        : public mpl::true_
    { };

    template<
        typename TDeps
      , typename TGiven
      , bool Assert
    >
    class verify<
        TDeps
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

