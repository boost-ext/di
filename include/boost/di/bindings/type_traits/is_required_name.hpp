//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_TYPE_TRAITS_IS_REQUIRED_NAME_HPP
#define BOOST_DI_BINDINGS_TYPE_TRAITS_IS_REQUIRED_NAME_HPP

#include "boost/di/aux_/mpl.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

BOOST_DI_HAS_MEMBER_TYPE(name);

template<typename TName>
class is_required_name {
    template<typename T, typename = void>
    struct get_name {
        struct no_name { };
        using type = no_name;
    };

    template<typename T>
    struct get_name<T, typename std::enable_if<
        has_name<typename di::type_traits::remove_accessors<T>::type>::value>::type
    > {
        using type = typename di::type_traits::remove_accessors<T>::type::name;
    };

public:
    template<typename T>
    using apply = int_<std::is_same<typename get_name<typename T::type>::type, TName>::value>;

    template<typename>
    using eval = int_<1>;
};

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

#endif

