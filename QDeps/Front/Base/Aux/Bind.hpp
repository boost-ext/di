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
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include "QDeps/Back/Dependency.hpp"
#include "QDeps/Utility/Named.hpp"
#include "QDeps/Front/Base/Aux/Internal.hpp"

namespace QDeps
{
namespace Front
{
namespace Base
{
namespace Aux
{

template<typename TExpected, typename TGiven = void> struct Bind : Internal, Back::Dependency
    <
        boost::mpl::_1,
        TExpected,
        TGiven,
        boost::mpl::vector0<>
    >
{
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct InCall : Internal, Back::Dependency
        <
            boost::mpl::_1,
            TExpected,
            TGiven,
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
        >
    {
        template<typename TName> struct InName : Internal, Back::Dependency
            <
                boost::mpl::_1,
                Utility::Named<TExpected, TName>,
                TGiven,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
            >
        { };
    };

    template<typename TName> struct InName : Internal, Back::Dependency
        <
            boost::mpl::_1,
            Utility::Named<TExpected, TName>,
            TGiven
        >
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct InCall : Back::Dependency
            <
                boost::mpl::_1,
                Utility::Named<TExpected, TName>,
                TGiven,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
            >
        { };
    };
};

template<typename TExpected> struct Bind<TExpected, void> : Internal, Back::Dependency
    <
        boost::mpl::_1,
        TExpected,
        TExpected,
        boost::mpl::vector0<>,
        boost::is_base_of<boost::mpl::_1, TExpected>
    >
{
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct InCall : Internal, Back::Dependency
        <
            boost::mpl::_1,
            TExpected,
            TExpected,
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
            boost::is_base_of<boost::mpl::_1, TExpected>
        >
    {
        template<typename TName> struct InName : Internal, Back::Dependency
            <
                boost::mpl::_1,
                Utility::Named<TExpected, TName>,
                TExpected,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                boost::is_base_of<boost::mpl::_1, Utility::Named<TExpected, TName> >
            >
        { };
    };

    template<typename TName> struct InName : Internal, Back::Dependency
        <
            boost::mpl::_1,
            Utility::Named<TExpected, TName>,
            TExpected,
            boost::mpl::vector0<>,
            boost::is_base_of<boost::mpl::_1, Utility::Named<TExpected, TName> >
        >
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct InCall : Internal, Back::Dependency
            <
                boost::mpl::_1,
                Utility::Named<TExpected, TName>,
                TExpected,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                boost::is_base_of<boost::mpl::_1, Utility::Named<TExpected, TName> >
            >
        { };
    };
};

} // namespace Aux
} // namespace Base
} // namespace Front
} // namespace QDeps

#endif

