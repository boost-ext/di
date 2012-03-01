//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_GENERIC_MODULE_HPP
#define QDEPS_FRONT_GENERIC_MODULE_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/void.hpp>
#include "QDeps/Front/Generic/Aux/Impl.hpp"
#include "QDeps/Front/Generic/Aux/Inst.hpp"
#include "QDeps/Front/Generic/Aux/Scope.hpp"
#include "QDeps/Front/Generic/Detail/Deps.hpp"
#include "QDeps/Front/Generic/Detail/Keys.hpp"
#include "QDeps/Utility/Injector.hpp"

namespace QDeps
{
namespace Front
{
namespace Generic
{

class Module
{
public:
    template
    <
        typename TIf,
        typename TImpl
    >
    class Impl : public Aux::Impl<TIf, TImpl>
    { };

    template
    <
        typename TAttr,
        typename TValue = boost::mpl::_1
    >
    class Inst : public Aux::Inst<TAttr, TValue>
    { };

    template<typename TScope>
    class Scope : public Aux::Scope<TScope>
    { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, boost::mpl::void_)>
    struct CallStack : BOOST_PP_CAT(boost::mpl::vector, BOOST_MPL_LIMIT_VECTOR_SIZE)<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
    { };

private:
    template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T)> friend class Utility::Injector;

    template<typename TBinding>
    struct Deps : Detail::Deps<TBinding>
    { };

    template<typename TBinding>
    struct Keys : Detail::Keys< Deps<TBinding> >
    { };
};

} // namespace Generic
} // namespace Front
} // namespace QDeps

#endif

