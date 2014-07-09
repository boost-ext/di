//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_SCOPES_PERMISSION_HPP
#define BOOST_DI_POLICIES_SCOPES_PERMISSION_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/scopes/deduce.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/bool.hpp>

namespace boost {
namespace di {
namespace policies {

template<typename TScope>
struct allow_scope
{
    template<typename T>
    struct allow
        : is_same<T, TScope>
    { };
};

template<>
struct allow_scope<scopes::deduce>; // disabled

/**
 * @code
 * scopes_permission<>
 * bind<deduce<int>> // compile error
 *
 * scopes_permission<allow_scope<scopes::unique>>
 * bind<unique<int>> // compile ok
 *
 * @endcode
 */
template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class scopes_permission
{
    typedef BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T) permitted_types;

    template<typename TAllow, typename T>
    struct is_scope_permitted_impl
        : TAllow::template allow<T>
    { };

    template<typename T>
    struct is_scope_permitted
        : mpl::bool_<
              mpl::count_if<
                  permitted_types
                , is_scope_permitted_impl<mpl::_, T>
              >::value != 0
          >
    { };

public:
    template<typename TDependency>
    void assert_policy() const {
        BOOST_DI_ASSERT_MSG(
            is_scope_permitted<typename TDependency::scope>::value
          , SCOPE_NOT_PERMITTED
          , typename TDependency::scope
        );
    }
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

