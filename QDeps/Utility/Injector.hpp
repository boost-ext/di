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
#include <boost/mpl/vector.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include "QPool/Pool.hpp"
#include "QDeps/Back/Aux/Utility.hpp"
#include "QDeps/Back/Module.hpp"
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
    typedef boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> Seq;

    struct Modules : boost::mpl::remove_if<Seq, boost::is_base_of<Back::Detail::Policy, boost::mpl::_> >::type { };
    struct Polices : boost::mpl::joint_view
        <
            boost::mpl::filter_view<Seq, boost::is_base_of<Back::Detail::Policy, boost::mpl::_> >,
            boost::mpl::vector1<typename Defaults<Back::Detail::Policy, Specialized>::type>
        >::type
    { };

    template<typename TSeq, typename TResult = boost::mpl::set0<> >
    struct DependenciesImpl : boost::mpl::fold
        <
            TSeq,
            TResult,
            boost::mpl::if_
            <
                boost::is_base_of<Back::Module, boost::mpl::_2>,
                DependenciesImpl<Back::Aux::GetDependencies<boost::mpl::_2>, boost::mpl::_1>,
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
                ExternalsImpl<Back::Aux::GetExternals<boost::mpl::_2>, boost::mpl::_1>,
                boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
            >
        >
    { };

public:
    struct Dependencies : boost::mpl::fold
        <
            typename DependenciesImpl<Modules>::type,
            boost::mpl::vector0<>,
            boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
        >::type
    { };


    struct Externals : boost::mpl::fold
        <
            typename ExternalsImpl<Modules>::type,
            boost::mpl::vector0<>,
            boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
        >::type
    { };

private:
    typedef QPool::Pool<typename Externals::type> Pool;
    typedef typename boost::mpl::deref<typename boost::mpl::begin<Polices>::type>::type Policy;
    typedef Back::Factory<typename Dependencies::type, Pool, Policy> Factory;

public:
/*    QPOOL_CTOR(Injector,*/
        //(m_pool)
        //(m_factory(m_pool)),
    /*{ })*/
    Injector()
        : m_factory(m_pool)
    { }

    template<typename M0> Injector(const M0& p0)
        : m_pool(p0.pool()),
          m_factory(m_pool)
    { }

    template<typename M0, typename M1> Injector(const M0& p0, const M1& p1)
        : m_pool(p0.pool(), p1.pool()),
          m_factory(m_pool)
    { }

    template<typename T> T create()
    {
        return m_factory.create<T>();
    }

    template<typename T, typename Visitor> void visit(const Visitor& p_visitor)
    {
        return m_factory.visit<T>(p_visitor);
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

