//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_BASE_SCOPE_HPP
#define QDEPS_FRONT_BASE_SCOPE_HPP

#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/has_xxx.hpp>
#include "QDeps/Back/Dependency.hpp"

namespace QDeps
{
namespace Front
{
namespace Base
{
namespace Aux
{

template<typename TScope>
struct Scope
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(Bind)

    template
    <
        typename T,
        typename TContext = boost::mpl::vector0<>
    >
    struct Dependency : Back::Dependency
        <
            boost::mpl::_1,
            T,
            T,
            TContext,
            boost::is_base_of<boost::mpl::_1, boost::mpl::_2>
        >
    { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct Bind : boost::mpl::fold
        <
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
            boost::mpl::vector0<>,
            boost::mpl::if_
            <
                has_Bind<boost::mpl::_2>,
                boost::mpl::push_back<boost::mpl::_1, Back::Apply<boost::mpl::_2, TScope> >,
                boost::mpl::push_back<boost::mpl::_1, Back::Apply<Dependency<boost::mpl::_2>, TScope> >
            >
        >::type
    { };

    template<typename TExpected>
    struct Bind<TExpected, BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(BOOST_MPL_LIMIT_VECTOR_SIZE, 1), mpl_::na BOOST_PP_INTERCEPT)> : boost::mpl::fold
        <
            boost::mpl::vector1<TExpected>,
            boost::mpl::vector0<>,
            boost::mpl::if_
            <
                has_Bind<boost::mpl::_2>,
                boost::mpl::push_back<boost::mpl::_1, Back::Apply<boost::mpl::_2, TScope> >,
                boost::mpl::push_back<boost::mpl::_1, Back::Apply< Dependency<boost::mpl::_2>, TScope> >
            >
        >::type
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct InCall : boost::mpl::fold
            <
                boost::mpl::vector1<TExpected>,
                boost::mpl::vector0<>,
                boost::mpl::if_
                <
                    has_Bind<boost::mpl::_2>,
                    boost::mpl::push_back<boost::mpl::_1, Back::Apply<boost::mpl::_2, TScope> >,
                    boost::mpl::push_back<boost::mpl::_1, Back::Apply< Dependency<boost::mpl::_2, boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> >, TScope> >
                >
            >::type
        { };

        //struct InName : Back::Dependency
        //{}
    };
};

} // namespace Aux
} // namespace Base
} // namespace Front
} // namespace QDeps

#endif

