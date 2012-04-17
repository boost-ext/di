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
    #include <boost/type_traits/is_same.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/find_if.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/transform.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include "di/Back/Aux/Pool.hpp"
    #include "di/Back/Aux/Utility.hpp"
    #include "di/Back/Aux/Instance.hpp"
    #include "di/Back/Module.hpp"
    #include "di/Back/Scopes/Singleton.hpp"
    #include "di/Back/Scopes/PerRequest.hpp"
    #include "di/Front/Base/Aux/Internal.hpp"
    #include "di/Front/Base/Aux/Scope.hpp"
    #include "di/Front/Base/Aux/Bind.hpp"
    #include "di/Front/Base/Aux/Externals.hpp"
    #include "di/Front/Base/Aux/Annotate.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_MPL_LIMIT_VECTOR_SIZE, "di/Front/Base/Module.hpp"))

    namespace di
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

    template<typename T> struct Annotate : Aux::Annotate<T> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    class Module : public Back::Module
    {
        template<typename TInstance, typename T> struct IsSameInstance : boost::mpl::or_
            <
                boost::is_same<typename TInstance::Name, T>,
                boost::is_same<typename TInstance::ValueType, T>
            >
        { };

        template<typename TSeq, typename T> struct FindInstanceType
            : boost::mpl::find_if<TSeq, IsSameInstance<boost::mpl::_1, T> >::type
        { };

        template<typename T, typename Enable = void>
        struct MakeAnnotation
        {
            typedef typename Annotate<Back::Aux::Instance<T> >::template With<> type;
        };

        template<typename T>
        struct MakeAnnotation<T, typename boost::enable_if<boost::is_base_of<Base::Aux::Internal, T> >::type>
        {
            typedef typename T::template Rebind<Back::Scopes::Singleton>::type Dependency;
            typedef Back::Aux::Instance<typename Dependency::Expected, typename Dependency::Context> Instance;
            typedef typename Annotate<Instance>::template With<typename T::Name> type;
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
                MakeAnnotation<boost::mpl::_1>
            >::type
        { };

        struct Instances : boost::mpl::transform
            <
                Externals,
                Back::Aux::GetDerived<boost::mpl::_1>
            >::type
        { };

    public:
        typedef Back::Aux::Pool<typename Instances::type> Pool;

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

        template<typename T, typename TValue>
        inline static typename boost::disable_if<boost::is_same<FindInstanceType<Externals, T>, boost::mpl::end<Externals> >, typename FindInstanceType<Externals, T>::type::Derived>::type
        Set(TValue p_value)
        {
            typedef typename FindInstanceType<Externals, T>::type Annotation;
            return typename Annotation::Derived(p_value);
        }

        const Pool& pool() const { return m_pool; }

    private:
        Pool m_pool;
    };

    } // namespace Base
    } // namespace Front
    } // namespace di

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    Module(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg))
        : m_pool(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg))
    { }

#endif

