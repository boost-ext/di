//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_UTILITY_INJECTOR_HPP
#define QDEPS_UTILITY_INJECTOR_HPP

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include "QPool/Pool.hpp"
#include "QDeps/Back/Module.hpp"
#include "QDeps/Back/Utility.hpp"
#include "QDeps/Back/Factory.hpp"
#include "QDeps/Back/Policy.hpp"
#include "QPool/Utility/Ctor.hpp"
#include "QDeps/Config.hpp"

namespace QDeps
{
namespace Utility
{

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
class Injector
{
    struct Policy : Back::Detail::Parameter
        <
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
            typename Defaults<Detail::Policy>::type,
            boost::is_base_of<Detail::Policy, boost::mpl::_2>
        >
    { };

    template<typename TSeq, typename TResult = boost::mpl::set0<> >
    struct DependenciesImpl : boost::mpl::fold
        <
            TSeq,
            TResult,
            boost::mpl::if_
            <
                boost::is_base_of<Back::Module, boost::mpl::_2>,
                DependenciesImpl<Back::GetDependencies<boost::mpl::_2>, boost::mpl::_1>,
                boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
            >
        >
    { };

    template<typename TSeq, typename TResult = boost::mpl::set0<> >
    struct ExternalsImpl : boost::mpl::fold
        <
            TSeq,
            TResult,
            boost::mpl::if_
            <
                boost::is_base_of<Back::Module, boost::mpl::_2>,
                ExternalsImpl<Back::GetExternals<boost::mpl::_2>, boost::mpl::_1>,
                boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
            >
        >
    { };

public:
    struct Dependencies : boost::mpl::fold
        <
            typename DependenciesImpl< boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> >::type,
            boost::mpl::vector0<>,
            boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
        >::type
    { };


    struct Externals : boost::mpl::fold
        <
            typename ExternalsImpl< boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> >::type,
            boost::mpl::vector0<>,
            boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
        >::type
    { };

private:
    typedef QPool::Pool<typename Externals::type> Pool;
    typedef Back::Factory<typename Dependencies::type, Pool> Factory;

public:
    QPOOL_CTOR(Injector,
        (m_pool)
        (m_factory(m_pool)),
    { })

    template<typename T> T create()
    {
        return m_factory.create<T>();
    }

    template<typename TVisitor> void visit(const TVisitor&)
    {
    }

/*    template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, typename M)> Injector<M...> install(T...)*/
    //{
    /*}*/

private:
    Pool m_pool;
    Factory m_factory;
};

} // namespace Utility
} // namespace QDeps

#endif

