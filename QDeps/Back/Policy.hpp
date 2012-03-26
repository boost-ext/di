//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_POLICY_HPP
#define QDEPS_BACK_POLICY_HPP

#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "QDeps/Config.hpp"

namespace QDeps
{
namespace Back
{

namespace Detail
{
template<typename TParameters, typename TDefault, typename TLambda>
struct Parameter : boost::mpl::fold
    <
        TParameters,
        TDefault,
        boost::mpl::if_
        <
            TLambda,
            boost::mpl::_2,
            boost::mpl::_1
        >
    >
{ };
} // namespace Detail

template<typename T0, typename T1, typename T2>
class Policy : QDeps::Detail::Policy
{
    template<typename T>
    struct ParameterImpl : Detail::Parameter
        <
            boost::mpl::vector<T0, T1, T2>,
            typename Defaults<T>::type,
            boost::is_base_of<T, boost::mpl::_2>
        >::type
    { };

public:
    typedef typename ParameterImpl<QDeps::Detail::CircularDependencies>::type CircularDependencies;
    typedef typename ParameterImpl<QDeps::Detail::BindingWithCtors>::type BindingWithCtors;
    typedef typename ParameterImpl<QDeps::Detail::ModulesDependeciesRepetitions>::type ModulesDependeciesRepetitions;
};

} // namespace Back
} // namespace QDeps

#endif

