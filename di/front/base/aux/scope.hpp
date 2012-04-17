//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_FRONT_BASE_SCOPE_HPP
#define DI_FRONT_BASE_SCOPE_HPP

#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/push_back.hpp>
#include "di/back/aux/dependency.hpp"
#include "di/back/aux/utility.hpp"
#include "di/utility/named.hpp"
#include "di/front/base/aux/internal.hpp"

namespace di
{
namespace Front
{
namespace Base
{
namespace Aux
{

template<typename TScope>
class Scope
{
    template<typename TRebind, typename T>
    struct Rebind : TRebind::template Rebind<T>
    { };

public:
    template
    <
        typename TExpected,
        typename TGiven,
        typename TContext = boost::mpl::vector0<>
    >
    struct Dependency : Internal, Back::Aux::Dependency
        <
            boost::mpl::_1,
            TExpected,
            TGiven,
            TContext,
            boost::mpl::or_< boost::is_base_of<boost::mpl::_1, TExpected>, boost::is_same<boost::mpl::_1, TExpected> >
        >
    { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct Bind : Internal, boost::mpl::fold
        <
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
            boost::mpl::vector0<>,
            boost::mpl::if_
            <
                boost::is_base_of<Internal, boost::mpl::_2>,
                boost::mpl::push_back<boost::mpl::_1, Rebind<boost::mpl::_2, TScope> >,
                boost::mpl::push_back<boost::mpl::_1, Rebind<Dependency<boost::mpl::_2, boost::mpl::_2>, TScope> >
            >
        >::type
    { };

    template<typename TExpected>
    struct Bind<TExpected, BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(BOOST_MPL_LIMIT_VECTOR_SIZE, 1), mpl_::na BOOST_PP_INTERCEPT)> : Internal, boost::mpl::fold
        <
            boost::mpl::vector1<TExpected>,
            boost::mpl::vector0<>,
            boost::mpl::if_
            <
                boost::is_base_of<Internal, boost::mpl::_2>,
                boost::mpl::push_back<boost::mpl::_1, Rebind<boost::mpl::_2, TScope> >,
                boost::mpl::push_back<boost::mpl::_1, Rebind< Dependency<boost::mpl::_2, boost::mpl::_2>, TScope> >
            >
        >::type
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct InCallStack : Internal, boost::mpl::fold
            <
                boost::mpl::vector1<TExpected>,
                boost::mpl::vector0<>,
                boost::mpl::if_
                <
                    boost::is_base_of<Internal, boost::mpl::_2>,
                    boost::mpl::push_back<boost::mpl::_1, Rebind<boost::mpl::_2, TScope> >,
                    boost::mpl::push_back<boost::mpl::_1, Rebind< Dependency<boost::mpl::_2, boost::mpl::_2, boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> >, TScope> >
                >
            >::type
        {
            template<typename TName>
            struct InName : Internal, boost::mpl::fold
                <
                    boost::mpl::vector1<TExpected>,
                    boost::mpl::vector0<>,
                    boost::mpl::if_
                    <
                        boost::is_base_of<Internal, boost::mpl::_2>,
                        boost::mpl::push_back<boost::mpl::_1, Rebind<boost::mpl::_2, TScope> >,
                        boost::mpl::push_back<boost::mpl::_1, Rebind< Dependency<Utility::Named<boost::mpl::_2, TName>, boost::mpl::_2, boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> >, TScope> >
                    >
                >::type
            { };
        };

        template<typename TName>
        struct InName : Internal, boost::mpl::fold
            <
                boost::mpl::vector1<TExpected>,
                boost::mpl::vector0<>,
                boost::mpl::if_
                <
                    boost::is_base_of<Internal, boost::mpl::_2>,
                    boost::mpl::push_back<boost::mpl::_1, Rebind<boost::mpl::_2, TScope> >,
                    boost::mpl::push_back<boost::mpl::_1, Rebind< Dependency<Utility::Named<boost::mpl::_2, TName>, boost::mpl::_2>, TScope> >
                >
            >::type
        {
            template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
            struct InCallStack : Internal, boost::mpl::fold
                <
                    boost::mpl::vector1<TExpected>,
                    boost::mpl::vector0<>,
                    boost::mpl::if_
                    <
                        boost::is_base_of<Internal, boost::mpl::_2>,
                        boost::mpl::push_back<boost::mpl::_1, Rebind<boost::mpl::_2, TScope> >,
                        boost::mpl::push_back<boost::mpl::_1, Rebind< Dependency<Utility::Named<boost::mpl::_2, TName>, boost::mpl::_2, boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> >, TScope> >
                    >
                >::type
            { };
        };
    };
};

} // namespace Aux
} // namespace Base
} // namespace Front
} // namespace di

#endif

