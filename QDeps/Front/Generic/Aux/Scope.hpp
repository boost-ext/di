//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_GENERIC_AUX_SCOPE_HPP
#define QDEPS_FRONT_GENERIC_AUX_SCOPE_HPP

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
#include <boost/mpl/is_sequence.hpp>

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
    typename TScope,
    typename T
>
struct ScopeDependency
{
    template<typename> struct Apply
    {
        typedef typename T::template Apply<TScope>::type type;
    };
};

} // namespace Detail

template<typename TScope>
class Scope
{
    template<typename T> struct Scope_
    {
        typedef Detail::ScopeDependency<TScope, T> type;
    };

    template<typename TSeq, typename TResult = boost::mpl::vector0<> >
    struct ScopeImpl : boost::mpl::fold
        <
            TSeq,
            TResult,
            boost::mpl::if_
            <
                boost::is_base_of<boost::mpl::void_, boost::mpl::_2>,
                boost::mpl::_1,
                boost::mpl::if_
                <
                    boost::mpl::is_sequence<boost::mpl::_2>,
                    ScopeImpl<boost::mpl::_2, boost::mpl::_1>,
                    boost::mpl::push_back<boost::mpl::_1, Scope_<boost::mpl::_2> >
                >
            >
        >
    { };

public:
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T_, boost::mpl::void_)>
    struct Bind : ScopeImpl
        <
            BOOST_PP_CAT(boost::mpl::vector, BOOST_MPL_LIMIT_VECTOR_SIZE)<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T_)>
        >::type
    { };
};

} // namespace Aux
} // namespace Generic
} // namespace Front
} // namespace QDeps

#endif

