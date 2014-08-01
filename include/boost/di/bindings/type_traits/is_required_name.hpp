//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_TYPE_TRAITS_IS_REQUIRED_NAME_HPP
#define BOOST_DI_BINDINGS_TYPE_TRAITS_IS_REQUIRED_NAME_HPP

#include "boost/di/type_traits/remove_accessors.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

BOOST_MPL_HAS_XXX_TRAIT_DEF(name)

template<typename TName>
class is_required_name
{
    template<typename T, typename = void>
    struct get_name
    {
        struct no_name { };
        typedef no_name type;
    };

    template<typename T>
    struct get_name<T, typename enable_if<
        has_name<typename di::type_traits::remove_accessors<T>::type> >::type
    >
    {
        typedef typename di::type_traits::remove_accessors<T>::type::name type;
    };

public:
    template<typename T>
    struct apply
        : is_same<typename get_name<typename T::type>::type, TName>
    { };
};

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

#endif

