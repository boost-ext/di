//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_GENERIC_AUX_INSTANCE_HPP
#define QDEPS_FRONT_GENERIC_AUX_INSTANCE_HPP

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/void.hpp>
#include "QDeps/Back/Utility.hpp"
#include "QDeps/Back/Aux/Inst.hpp"

namespace QDeps
{
namespace Front
{
namespace Generic
{
namespace Aux
{

namespace Detail
{

template
<
    typename TAttr,
    typename TValue,
    typename TContext = boost::mpl::vector0<>
>
struct InstanceDependency
{
    template<typename TScope> struct Apply
    {
        typedef Back::Aux::Inst<TScope, TAttr, TValue, TContext> type;
    };
};

} // namespace Detail

template
<
    typename TAttr,
    typename TValue = boost::mpl::void_
>
class Instance : public Detail::InstanceDependency<TAttr, TValue>
{
    template<typename T> struct Instance_
    {
        typedef Detail::InstanceDependency<TAttr, TValue, typename Back::MakeVector<T>::type> type;
    };

public:
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct Bind : boost::mpl::fold
        <
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
            boost::mpl::vector0<>,
            boost::mpl::push_back<boost::mpl::_1, Instance_<boost::mpl::_2> >
        >::type
    { };
};

} // namespace Aux
} // namespace Generic
} // namespace Front
} // namespace QDeps

#endif

