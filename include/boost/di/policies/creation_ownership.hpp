//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CREATION_OWNERSHIP_HPP
#define BOOST_DI_POLICIES_CREATION_OWNERSHIP_HPP

#include "boost/di/aux_/config.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_reference.hpp>

namespace boost {
namespace di {
namespace policies {

/**
 * @code
 * injector_.create<const int&>(); // compile error
 * injector_.create<int&>(); // compile error
 * injector_.create<int>(); // compile ok
 * @endcode
 */
class creation_ownership
{
    template<typename TDependency>
    struct is_result_type_reference
        : mpl::bool_<
             mpl::size<typename TDependency::call_stack>::value == 1 &&
             is_reference<typename TDependency::type>::value
          >
    { };

public:
    template<typename TDependency>
    typename disable_if<is_result_type_reference<TDependency> >::type
    assert_policy() const { }

    template<typename TDependency>
    typename enable_if<is_result_type_reference<TDependency> >::type
    assert_policy() const {
        BOOST_DI_ASSERT_MSG(
            false
          , CREATION_OWNERSHIP_IS_NOT_CLEAR
          , typename TDependency::type
        );
    }
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

