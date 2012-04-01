//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_POLICIES_DISALLOWCIRCULARDEPENDENCIES_HPP
#define QDEPS_BACK_POLICIES_DISALLOWCIRCULARDEPENDENCIES_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/equal.hpp>

namespace QDeps
{
namespace Back
{
namespace Policies
{

class DisallowCircularDependencies
{
    template<typename TSeq> struct IsUnique
        : boost::mpl::equal<TSeq, typename boost::mpl::unique<TSeq, boost::is_same<boost::mpl::_1, boost::mpl::_2> >::type>
    { };

public:
    template<typename, typename TCallStack, typename>
    void operator()(typename boost::enable_if<IsUnique<TCallStack> >::type* = 0) const { }

    template<typename T, typename TCallStack, typename>
    void operator()(typename boost::disable_if<IsUnique<TCallStack> >::type* = 0) const
    {
        BOOST_MPL_ASSERT_MSG(
            false,
            CIRCULAR_DEPENDENCIES_DISALLOWED,
            (T, TCallStack)
        );
    }
};

} // namespace Policies
} // namespace Back
} // namespace QDeps

#endif

