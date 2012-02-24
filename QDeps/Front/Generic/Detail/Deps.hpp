//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_GENERIC_DETAIL_DEPS_HPP
#define QDEPS_FRONT_GENERIC_DETAIL_DEPS_HPP

#include <boost/mpl/fold.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include "QDeps/Utility/Scopes/PerRequest.hpp"

namespace QDeps
{
namespace Front
{
namespace Generic
{
namespace Detail
{

namespace Detail
{

template<typename TDep> struct CreateDependency
{
    typedef typename TDep::template Apply<Utility::Scopes::PerRequest>::type type;
};

template<typename TBinding, typename TSeq = boost::mpl::set0<> >
struct DepsImpl : boost::mpl::fold
    <
        TBinding,
        TSeq,
        boost::mpl::if_
        <
            boost::mpl::is_sequence<boost::mpl::_2>,
            DepsImpl<boost::mpl::_2, boost::mpl::_1>,
            boost::mpl::insert<boost::mpl::_1, CreateDependency<boost::mpl::_2> >
        >
    >
{ };

} // namespace Detail

template<typename TBinding>
struct Deps : boost::mpl::fold
    <
        typename Detail::DepsImpl<TBinding>::type,
        boost::mpl::vector0<>,
        boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
    >::type
{ };

} // namespace Detail
} // namespace Generic
} // namespace Front
} // namespace QDeps

#endif

