//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_FRONT_BASE_MODULE_HPP
    #define QDEPS_FRONT_BASE_MODULE_HPP

    #include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/transform.hpp>
    #include "QDeps/Back/Aux/Pool.hpp"
    #include "QDeps/Back/Aux/Utility.hpp"
    #include "QDeps/Back/Aux/Instance.hpp"
    #include "QDeps/Back/Module.hpp"
    #include "QDeps/Back/Scopes/Singleton.hpp"
    #include "QDeps/Back/Scopes/PerRequest.hpp"
    #include "QDeps/Front/Base/Aux/Internal.hpp"
    #include "QDeps/Front/Base/Aux/Scope.hpp"
    #include "QDeps/Front/Base/Aux/Bind.hpp"
    #include "QDeps/Front/Base/Aux/Externals.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_MPL_LIMIT_VECTOR_SIZE, "QDeps/Front/Base/Module.hpp"))

    namespace QDeps
    {
    namespace Front
    {
    namespace Base
    {

    template<typename TScope> struct Scope : Aux::Scope<TScope> { };
    template<typename TExpected, typename TGiven = TExpected> struct Bind : Aux::Bind<TExpected, TGiven> { };

    template<typename TExpected, typename TGiven = TExpected> struct Singleton : Scope<Back::Scopes::Singleton>::Bind< Bind<TExpected, TGiven> > { };
    template<typename T> struct Singleton<T, T> : Scope<Back::Scopes::Singleton>::Bind<T> { };

    template<typename TExpected, typename TGiven = TExpected> struct PerRequest : Scope<Back::Scopes::PerRequest>::Bind< Bind<TExpected, TGiven> > { };
    template<typename T> struct PerRequest<T, T> : Scope<Back::Scopes::PerRequest>::Bind<T> { };

    template<typename T> struct External : Aux::Externals<T> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct Singletons : Scope<Back::Scopes::Singleton>::Bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct PerRequests : Scope<Back::Scopes::PerRequest>::Bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct Externals : Aux::Externals<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    class Module : Back::Module
    {
        template<typename T, typename Enable = void>
        struct MakeInstance
        {
            typedef Back::Aux::Instance<T> type;
        };

        template<typename T>
        struct MakeInstance<T, typename boost::enable_if<boost::is_base_of<Base::Aux::Detail::Internal, T> >::type>
        {
            typedef typename T::template Rebind<Back::Scopes::Singleton>::type Dependency;
            typedef Back::Aux::Instance<typename Dependency::Expected, typename Dependency::Context> type;
        };

        struct Externals : boost::mpl::transform
            <
                typename boost::mpl::fold
                <
                    boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                    boost::mpl::vector0<>,
                    boost::mpl::copy
                    <
                        boost::mpl::if_
                        <
                            boost::is_base_of<Aux::Detail::Externals, boost::mpl::_2>,
                            boost::mpl::_2,
                            boost::mpl::vector0<>
                        >,
                        boost::mpl::back_inserter<boost::mpl::_1>
                    >
                >::type,
                MakeInstance<boost::mpl::_1>
            >::type
        { };

    public:
        typedef Back::Aux::Pool<typename Externals::type> Pool;

        struct Dependencies : boost::mpl::fold
            <
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                boost::mpl::vector0<>,
                boost::mpl::copy
                <
                    boost::mpl::if_
                    <
                        boost::is_base_of<Aux::Detail::Externals, boost::mpl::_2>,
                        boost::mpl::vector0<>,
                        boost::mpl::if_
                        <
                            boost::mpl::is_sequence<boost::mpl::_2>,
                            boost::mpl::_2,
                            PerRequest<boost::mpl::_2>
                        >
                    >,
                    boost::mpl::back_inserter<boost::mpl::_1>
                >
            >::type
        { };

        Module() { }

        #include BOOST_PP_ITERATE()

    #if 0
        template<typename TName, typename T>
        inline Instance Set(const T& p_obj)
        {
            return Instance;
        }
    #endif

        const Pool& pool() const { return m_pool; }

    private:
        Pool m_pool;
    };

    } // namespace Base
    } // namespace Front
    } // namespace QDeps

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    Module(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg))
        : m_pool(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg))
    { }

#endif

