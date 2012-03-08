//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_GENERIC_DETAIL_KEYS_HPP
#define QDEPS_FRONT_GENERIC_DETAIL_KEYS_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/insert.hpp>
#include "QDeps/Back/Utility.hpp"

namespace QDeps
{
namespace Front
{
namespace Generic
{
namespace Detail
{

template<typename TDeps>
struct Keys : boost::mpl::fold
    <
        typename boost::mpl::fold
        <
            TDeps,
            boost::mpl::set0<>,
            boost::mpl::if_
            <
                Back::HasExternalValue<boost::mpl::_2>,
                boost::mpl::insert<boost::mpl::_1, Back::GetDependency<boost::mpl::_2> >,
                boost::mpl::_1
            >
        >::type,
        boost::mpl::vector0<>,
        boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
    >::type
{ };

} // namespace Detail
} // namespace Generic
} // namespace Front
} // namespace QDeps

#endif

