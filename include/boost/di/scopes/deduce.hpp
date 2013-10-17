//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_DEDUCE_HPP
#define BOOST_DI_SCOPES_DEDUCE_HPP

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
    template<typename T, typename = T>
    struct scope
    {
        typedef T result_type;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

