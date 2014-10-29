//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_DEDUCE_HPP
#define BOOST_DI_SCOPES_DEDUCE_HPP

#include "boost/di/type_traits/scope_traits.hpp"

#include <boost/mpl/int.hpp>

namespace boost {
namespace di {
namespace scopes {

/**
 * virtual scope -> converted to the other scope
 * accordingly to the given type using scope_traits
 */
class deduce
{
public:
    typedef mpl::int_<0> priority;

    template<typename>
    struct scope
    {
        typedef void result_type;
    };

    template<typename T>
    struct rebind
    {
        typedef typename type_traits::scope_traits<T>::type other;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

