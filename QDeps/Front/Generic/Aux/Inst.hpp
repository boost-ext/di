//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_GENERIC_AUX_INST_HPP
#define QDEPS_FRONT_GENERIC_AUX_INST_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/vector.hpp>
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
struct InstDependency
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
    typename TValue = boost::mpl::_1
>
class Inst : public Detail::InstDependency<TAttr, TValue>
{
    template<typename T> struct Inst_
    {
        typedef Detail::InstDependency<TAttr, TValue, T> type;
    };

    template<typename TSeq>
    struct InstImpl : boost::mpl::fold
        <
            TSeq,
            boost::mpl::vector0<>,
            boost::mpl::if_
            <
                boost::is_base_of<boost::mpl::void_, boost::mpl::_2>,
                boost::mpl::_1,
                boost::mpl::push_back<boost::mpl::_1, Inst_<boost::mpl::_2> >
            >
        >
    { };

public:
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, boost::mpl::void_)>
    struct Bind : InstImpl
        <
            BOOST_PP_CAT(boost::mpl::vector, BOOST_MPL_LIMIT_VECTOR_SIZE)<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
        >::type
    { };
};

} // namespace Aux
} // namespace Generic
} // namespace Front
} // namespace QDeps

#endif

