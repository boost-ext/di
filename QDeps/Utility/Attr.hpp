//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_UTILITY_ATTR_HPP
#define QDEPS_UTILITY_ATTR_HPP

namespace QDeps
{
namespace Utility
{

/**
 * Example: Attr< int, mpl::string<'Port'> >
 */
template<typename T, typename TName>
struct Attr
{
    //TODO non explicit ctor
    //TODO operator T()
};

} // namespace Utility
} // namespace QDeps

#endif

