//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_BASE_BIND_HPP
#define QDEPS_FRONT_BASE_BIND_HPP

#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include "di/Back/Aux/Dependency.hpp"
#include "di/Back/Aux/Value.hpp"
#include "di/Utility/Named.hpp"
#include "di/Front/Base/Aux/Internal.hpp"

namespace di
{
namespace Front
{
namespace Base
{
namespace Aux
{

template<typename TExpected, typename TGiven = TExpected, typename Enable = void>
struct Bind : Internal, Back::Aux::Dependency
    <
        boost::mpl::_1,
        TExpected,
        TGiven,
        boost::mpl::vector0<>
    >
{
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct InCallStack : Internal, Back::Aux::Dependency
        <
            boost::mpl::_1,
            TExpected,
            TGiven,
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
        >
    {
        template<typename TName>
        struct InName : Internal, Back::Aux::Dependency
            <
                boost::mpl::_1,
                Utility::Named<TExpected, TName>,
                TGiven,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
            >
        { };
    };

    template<typename TName>
    struct InName : Internal, Back::Aux::Dependency
        <
            boost::mpl::_1,
            Utility::Named<TExpected, TName>,
            TGiven
        >
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct InCallStack : Internal, Back::Aux::Dependency
            <
                boost::mpl::_1,
                Utility::Named<TExpected, TName>,
                TGiven,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
            >
        { };
    };
};

template<typename TExpected, typename TGiven>
struct Bind<TExpected, TGiven, typename boost::enable_if< boost::mpl::or_<Back::Aux::Value<TGiven>, boost::is_same<TExpected, TGiven> > >::type> : Internal, Back::Aux::Dependency
    <
        boost::mpl::_1,
        TExpected,
        TGiven,
        boost::mpl::vector0<>,
        boost::mpl::or_< boost::is_base_of<boost::mpl::_1, TExpected>, boost::is_same<boost::mpl::_1, TExpected> >
    >
{
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct InCallStack : Internal, Back::Aux::Dependency
        <
            boost::mpl::_1,
            TExpected,
            TGiven,
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
            boost::mpl::or_< boost::is_base_of<boost::mpl::_1, TExpected>, boost::is_same<boost::mpl::_1, TExpected> >
        >
    {
        template<typename TName>
        struct InName : Internal, Back::Aux::Dependency
            <
                boost::mpl::_1,
                Utility::Named<TExpected, TName>,
                TGiven,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                boost::mpl::or_< boost::is_base_of<boost::mpl::_1, Utility::Named<TExpected, TName> >, boost::is_same<boost::mpl::_1, Utility::Named<TExpected, TName> > >
            >
        { };
    };

    template<typename TName>
    struct InName : Internal, Back::Aux::Dependency
        <
            boost::mpl::_1,
            Utility::Named<TExpected, TName>,
            TGiven,
            boost::mpl::vector0<>,
            boost::mpl::or_< boost::is_base_of<boost::mpl::_1, Utility::Named<TExpected, TName> >, boost::is_same<boost::mpl::_1, Utility::Named<TExpected, TName> > >
        >
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct InCallStack : Internal, Back::Aux::Dependency
            <
                boost::mpl::_1,
                Utility::Named<TExpected, TName>,
                TGiven,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                boost::mpl::or_< boost::is_base_of<boost::mpl::_1, Utility::Named<TExpected, TName> >, boost::is_same<boost::mpl::_1, Utility::Named<TExpected, TName> > >
            >
        { };
    };
};

} // namespace Aux
} // namespace Base
} // namespace Front
} // namespace di

#endif

