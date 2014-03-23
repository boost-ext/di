//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_DEDUCE_HPP
#define BOOST_DI_SCOPES_DEDUCE_HPP

#include <boost/none_t.hpp>
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

    template<typename, typename>
    struct scope
    {
        typedef scope type;
        typedef none_t result_type;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

