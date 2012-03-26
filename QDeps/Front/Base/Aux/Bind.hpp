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

namespace QDeps
{
namespace Front
{
namespace Base
{
namespace Aux
{

template<typename TExpected, typename TGiven = void> struct Bind : Back::Dependency
    <
        boost::mpl::_1,
        TExpected,
        TGiven,
        boost::mpl::vector0<>
    >
{
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct InCall : Back::Dependency
        <
            boost::mpl::_1,
            TExpected,
            TGiven,
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
        >
    { };

    //struct InName : Back::Dependency
    //{}
};

template<typename TExpected> struct Bind<TExpected, void> : Back::Dependency
    <
        boost::mpl::_1,
        TExpected,
        TExpected,
        boost::mpl::vector0<>,
        boost::is_base_of<boost::mpl::_1, TExpected>
    >
{
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct InCall : Back::Dependency
        <
            boost::mpl::_1,
            TExpected,
            TExpected,
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
            boost::is_base_of<boost::mpl::_1, TExpected>
        >
    { };

    //struct InName : Back::Dependency
    //{}
};

} // namespace Aux
} // namespace Base
} // namespace Front
} // namespace QDeps

#endif

