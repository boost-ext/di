//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_DETAIL_CREATOR_HPP
#define QDEPS_BACK_DETAIL_CREATOR_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace QDeps
{
namespace Back
{
namespace Detail
{

namespace Detail
{
template<typename T> struct GetDependency
{
    typedef typename T::Dependency type;
};
} // namespace Detail

template<typename TElem, typename TDeps>
struct Creator : boost::mpl::fold
    <
        TDeps,
        boost::mpl::vector0<>,
        boost::mpl::if_
        <
            boost::is_same<TElem, Detail::GetDependency<boost::mpl::_2> >,
            boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>,
            boost::mpl::_1
        >
    >
{ };

} // namespace Back
} // namespace Detail
} // namespace QDeps

#endif

