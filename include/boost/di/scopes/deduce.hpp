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
 */
class deduce
{
public:
    template<typename, typename>
    class scope { };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

