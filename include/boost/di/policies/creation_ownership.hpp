//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CREATION_OWNERSHIP_HPP
#define BOOST_DI_POLICIES_CREATION_OWNERSHIP_HPP

#include "boost/di/aux_/common.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_reference.hpp>

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
        typename
      , typename T
    >
    static typename disable_if<is_reference<typename T::type> >::type assert_policy() { }

    template<
        typename
      , typename T
    >
    static typename enable_if<is_reference<typename T::type> >::type assert_policy() {
        BOOST_DI_ASSERT_MSG(
            false
          , CREATION_OWNERSHIP_IS_NOT_CLEAR
          , typename T::type
        );
    }
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

