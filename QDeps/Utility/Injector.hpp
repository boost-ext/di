//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_UTILITY_INJECTOR_HPP
#define QDEPS_UTILITY_INJECTOR_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_xxx.hpp>
#include "QPool/Pool.hpp"
#include "QPool/Utility/Ctor.hpp"
#include "QDeps/Back/Utility.hpp"
#include "QDeps/Back/Factory.hpp"
#include "QDeps/Back/Policy/Policy.hpp"
#include "QDeps/Config.hpp"

namespace QDeps
{
namespace Utility
{

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, boost::mpl::void_)>
class Injector
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(Binding)

    struct Modules : boost::mpl::fold
        <
            BOOST_PP_CAT(boost::mpl::vector, BOOST_MPL_LIMIT_VECTOR_SIZE)
            <
                BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)
            >,
            boost::mpl::vector0<>,
            boost::mpl::if_
            <
                boost::is_base_of<boost::mpl::void_, boost::mpl::_2>,
                boost::mpl::_1,
                boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
            >
        >
    { };

    struct Policy : Back::Policy::Detail::Parameter
        <
            typename Modules::type,
            typename Defaults<Detail::Policy>::type,
            boost::is_base_of<Detail::Policy, boost::mpl::_2>
        >
    { };

public:
    template<typename TSeq, typename TResult = boost::mpl::set0<> >
    struct DepsImpl : boost::mpl::fold
        <
            TSeq,
            TResult,
            boost::mpl::if_
            <
                has_Binding<boost::mpl::_2>,
                DepsImpl<Back::GetDeps<boost::mpl::_2>, boost::mpl::_1>,
                boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
            >
        >
    { };

    struct Deps : boost::mpl::fold
        <
            typename DepsImpl<typename Modules::type>::type,
            boost::mpl::vector0<>,
            boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
        >
    { };

    typedef boost::mpl::vector0<> Keys;

#if 0
    template</*template<typename> class GetImpl, */typename TSeq, typename TResult = boost::mpl::set0<> >
    struct BindingImpl : boost::mpl::fold
        <
            TSeq,
            TResult,
            boost::mpl::if_
            <
                has_Binding<boost::mpl::_2>,
                //BindingImpl<GetImpl, boost::mpl::identity<GetImpl<boost::mpl::_2> >, boost::mpl::_1>,
                BindingImpl</*GetImpl, */Back::GetDeps<boost::mpl::_2>, boost::mpl::_1>,
                boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
            >
        >::type
    { };

    //template<template<typename> class GetImpl>
    struct Binding : boost::mpl::fold
        <
            BindingImpl</*GetImpl, */typename Modules::type>,
            boost::mpl::vector0<>,
            boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
        >/*::type*/
    { };

public:
    typedef typename Binding::type/*<Back::GetDeps>*/ Deps;
    typedef typename Binding::type/*<Back::GetKeys>*/ Keys;
#endif

private:
    typedef QPool::Pool<Keys> Pool;
    typedef Back::Factory<Deps, Pool> Factory;

public:
    QPOOL_CTOR(Injector,
        (m_pool)
        (m_factory(m_pool)),
    { })

    template<typename T> T create()
    {
        return m_factory.create<T>();
    }

private:
    Pool m_pool;
    Factory m_factory;
};

} // namespace Utility
} // namespace QDeps

#endif

