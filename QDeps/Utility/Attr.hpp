//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_UTILITY_ATTR_HPP
#define QDEPS_UTILITY_ATTR_HPP

#include "QDeps/Config.hpp"

namespace QDeps
{
namespace Utility
{

/**
 * Example: Attr< int, mpl::string<'Port'> >
 */
template<typename T, typename TName>
class Attr
{
public:
    Attr(T p_value = *Defaults<T, Specialized>::create()) // non explicit
        : m_value(p_value)
    { }

    operator T() { return m_value; }

private:
    T m_value;
};

} // namespace Utility
} // namespace QDeps

#endif

