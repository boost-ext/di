//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CREATION_PERMISSION_HPP
#define BOOST_DI_POLICIES_CREATION_PERMISSION_HPP

#include "boost/di/aux_/config.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>

namespace boost {
namespace di {
namespace policies {

template<typename TValueType>
struct allow_type
{
    template<typename T>
    struct allow
        : is_same<T, TValueType>
    { };
};

template<typename TExpr>
struct allow_type_expr
{
    template<typename T>
    struct allow
        : mpl::apply<TExpr, T>::type
    { };
};

/**
 * @code
 * using namespace di::policies;
 *
 * di::make_injector().create<int>(creation_permission<>()); // compile error
 * di::make_injector(di::bind_int<42>()).create<int>(creation_permission<>()); // compile ok
 * di::make_injector().create<int>(creation_permission<allow_type<int>>()); // compile ok
 * di::make_injector().create<int>(creation_permission<allow_type_expr<is_pod<mpl::_>>>()); // compile ok
 * @endcode
 */
template<BOOST_DI_TYPES_DEFAULT_MPL(TArgs)>
class creation_permission
{
    typedef BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(TArgs) permitted_types;

    struct not_resolved
    {
        typedef not_resolved type;

        template<typename>
        struct rebind
        {
            typedef not_resolved other;
        };
    };

    template<typename TAllow, typename T>
    struct is_type_permitted_impl
        : TAllow::template allow<T>
    { };

    template<typename T>
    struct is_type_permitted
        : mpl::or_<
              mpl::not_<
                  is_same<
                      typename T::binder::template resolve<
                          typename T::type
                        , mpl::vector0<>
                        , not_resolved
                      >::type
                    , not_resolved
                  >
              >
            , mpl::bool_<
                  mpl::count_if<
                      permitted_types
                    , is_type_permitted_impl<mpl::_, typename T::type>
                  >::value != 0
              >
          >
    { };

public:
    template<typename T>
    void assert_policy() const {
       BOOST_DI_ASSERT_MSG(
            is_type_permitted<T>::value
          , TYPE_HAS_TO_BE_EXPLICITLY_BINDED
          , typename T::type
        );
    }
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

