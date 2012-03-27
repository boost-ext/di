//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_FRONT_FUSION_MODULE_HPP
    #define QDEPS_FRONT_FUSION_MODULE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
    #include <boost/typeof/typeof.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/if.hpp>
    #include <QPool/Utility/Ctor.hpp>
    #include <QPool/Pool.hpp>
    #include "QDeps/Back/Module.hpp"
    #include "QDeps/Back/Scope/Singleton.hpp"
    #include "QDeps/Back/Scope/PerRequest.hpp"
    #include "QDeps/Front/Base/Aux/Bind.hpp"
    #include "QDeps/Front/Base/Aux/Scope.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_MPL_LIMIT_VECTOR_SIZE, "QDeps/Front/Fusion/Module.hpp"))

    namespace QDeps
    {
    namespace Front
    {
    namespace Fusion
    {

    template<typename TScope> struct Scope : Base::Aux::Scope<TScope> { };
    template<typename TExpected, typename TGiven = TExpected> struct Bind : Base::Aux::Bind<TExpected, TGiven> { };

    template<typename TExpected, typename TGiven = TExpected> struct Singleton : Scope<Back::Scope::Singleton>::Bind< Bind<TExpected, TGiven> > { };
    template<typename T> struct Singleton<T, T> : Scope<Back::Scope::Singleton>::Bind<T> { };

    template<typename TExpected, typename TGiven = TExpected> struct PerRequest : Scope<Back::Scope::PerRequest>::Bind< Bind<TExpected, TGiven> > { };
    template<typename T> struct PerRequest<T, T> : Scope<Back::Scope::PerRequest>::Bind<T> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct Singletons : Scope<Back::Scope::Singleton>::Bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct PerRequests : Scope<Back::Scope::PerRequest>::Bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template
    <
        typename TSeq = boost::mpl::vector0<>
    >
    class Module : Back::Module
    {
    public:
        struct Dependencies : boost::mpl::fold
            <
                TSeq,
                boost::mpl::vector0<>,
                boost::mpl::copy
                <
                    boost::mpl::if_
                    <
                        boost::is_base_of<Base::Aux::Detail::Internal, boost::mpl::_2>,
                        boost::mpl::_2,
                        PerRequest<boost::mpl::_2>
                    >,
                    boost::mpl::back_inserter<boost::mpl::_1>
                >
            >::type
        { };

        struct Externals : boost::mpl::vector0<> { };

        QPOOL_CTOR(Module,
            (m_pool),
        { })

        #include BOOST_PP_ITERATE()

        const QPool::Pool<TSeq, QPool::Allocator::Stack>& pool() const { return m_pool; }

    private:
        QPool::Pool<TSeq, QPool::Allocator::Stack> m_pool;
    };

    } // namespace Fusion
    } // namespace Front
    } // namespace QDeps

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)> Module<boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), Arg)> >
    operator()(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg))
    {
        return Module< boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), Arg)> >(
            BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg));
    }

#endif

