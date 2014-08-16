//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_DEDUCE_HPP
#define BOOST_DI_SCOPES_DEDUCE_HPP

#include "boost/di/aux_/mpl.hpp"
#include "boost/di/type_traits/scope_traits.hpp"

namespace boost {
namespace di {
namespace scopes {

/**
 * virtual scope -> converted to the other scope
 * accordingly to the given type using scope_traits
 */
class deduce {
public:
    using priority = int_<0>;

    template<typename>
    struct scope {
        using result_type = none_t;
    };

    template<typename T>
    struct rebind {
        using other = typename type_traits::scope_traits<T>::type;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

