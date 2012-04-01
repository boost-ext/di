//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_POLICIES_DISALLOWCIRCULARDEPENDENCIES_HPP
#define QDEPS_BACK_POLICIES_DISALLOWCIRCULARDEPENDENCIES_HPP

#include <boost/mpl/assert.hpp>

namespace QDeps
{
namespace Back
{
namespace Policies
{

struct DisallowCircularDependencies
{
    template<typename TDeps> struct Init
    {
        BOOST_MPL_ASSERT_MSG(
            true,
            _,
            (TDeps)
        );
    };

    template<typename TDeps, typename T> struct Create { };
};

} // namespace Policies
} // namespace Back
} // namespace QDeps

#endif

