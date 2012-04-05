//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_POLICIES_CIRCULARDEPENDENCIES_HPP
#define QDEPS_BACK_POLICIES_CIRCULARDEPENDENCIES_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/count.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/size.hpp>
#include "QDeps/Config.hpp"

namespace QDeps
{
namespace Back
{
namespace Policies
{

class CircularDependencies
{
    template<typename TCallStack>
    struct IsUniqueCallStack : boost::mpl::bool_
        <
            boost::mpl::size<TCallStack>::value == boost::mpl::accumulate
            <
                typename boost::mpl::transform<TCallStack, boost::mpl::count<TCallStack, boost::mpl::_> >::type,
                boost::mpl::int_<0>,
                boost::mpl::plus< boost::mpl::_1, boost::mpl::_2>
            >::type::value
        >
    { };

public:
    //TODO T and check it with struct in compile time

    template<typename TCallStack>
    static void execute(typename boost::enable_if<IsUniqueCallStack<TCallStack> >::type* = 0) { }

    template<typename TCallStack>
    static void execute(typename boost::disable_if<IsUniqueCallStack<TCallStack> >::type* = 0)
    {
        QDEPS_STATIC_ASSERT(
            false,
            CIRCULAR_DEPENDENCIES_NOT_ALLOWED,
            (TCallStack)
        );
    }
};

} // namespace Policies
} // namespace Back
} // namespace QDeps

#endif

