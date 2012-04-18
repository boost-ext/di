//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_BIND_HPP
#define BOOST_DI_CONCEPTS_BIND_HPP

#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include "boost/di/aux/dependency.hpp"
#include "boost/di/aux/explicit_value.hpp"
#include "boost/di/aux/utility.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace concepts {

template<typename TExpected, typename TGiven = TExpected, typename Enable = void>
struct bind : aux::internal, aux::dependency
    <
        boost::mpl::_1,
        TExpected,
        TGiven,
        boost::mpl::vector0<>
    >
{
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct in_call_stack : aux::internal, aux::dependency
        <
            boost::mpl::_1,
            TExpected,
            TGiven,
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
        >
    {
        template<typename TName>
        struct in_name : aux::internal, aux::dependency
            <
                boost::mpl::_1,
                named<TExpected, TName>,
                TGiven,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
            >
        { };
    };

    template<typename TName>
    struct in_name : aux::internal, aux::dependency
        <
            boost::mpl::_1,
            named<TExpected, TName>,
            TGiven
        >
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct in_call_stack : aux::internal, aux::dependency
            <
                boost::mpl::_1,
                named<TExpected, TName>,
                TGiven,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
            >
        { };
    };
};

template<typename TExpected, typename TGiven>
struct bind<TExpected, TGiven, typename boost::enable_if< boost::mpl::or_<aux::explicit_value<TGiven>, boost::is_same<TExpected, TGiven> > >::type> : aux::internal, aux::dependency
    <
        boost::mpl::_1,
        TExpected,
        TGiven,
        boost::mpl::vector0<>,
        boost::mpl::or_< boost::is_base_of<boost::mpl::_1, TExpected>, boost::is_same<boost::mpl::_1, TExpected> >
    >
{
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct in_call_stack : aux::internal, aux::dependency
        <
            boost::mpl::_1,
            TExpected,
            TGiven,
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
            boost::mpl::or_< boost::is_base_of<boost::mpl::_1, TExpected>, boost::is_same<boost::mpl::_1, TExpected> >
        >
    {
        template<typename TName>
        struct in_name : aux::internal, aux::dependency
            <
                boost::mpl::_1,
                named<TExpected, TName>,
                TGiven,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                boost::mpl::or_< boost::is_base_of<boost::mpl::_1, named<TExpected, TName> >, boost::is_same<boost::mpl::_1, named<TExpected, TName> > >
            >
        { };
    };

    template<typename TName>
    struct in_name : aux::internal, aux::dependency
        <
            boost::mpl::_1,
            named<TExpected, TName>,
            TGiven,
            boost::mpl::vector0<>,
            boost::mpl::or_< boost::is_base_of<boost::mpl::_1, named<TExpected, TName> >, boost::is_same<boost::mpl::_1, named<TExpected, TName> > >
        >
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct in_call_stack : aux::internal, aux::dependency
            <
                boost::mpl::_1,
                named<TExpected, TName>,
                TGiven,
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                boost::mpl::or_< boost::is_base_of<boost::mpl::_1, named<TExpected, TName> >, boost::is_same<boost::mpl::_1, named<TExpected, TName> > >
            >
        { };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

