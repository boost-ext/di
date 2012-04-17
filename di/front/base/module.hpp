//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef DI_FRONT_BASE_MODULE_HPP
    #define DI_FRONT_BASE_MODULE_HPP

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
    #include "di/back/aux/pool.hpp"
    #include "di/back/aux/utility.hpp"
    #include "di/back/aux/instance.hpp"
    #include "di/back/module.hpp"
    #include "di/back/scopes/singleton.hpp"
    #include "di/back/scopes/per_request.hpp"
    #include "di/front/base/aux/internal.hpp"
    #include "di/front/base/aux/scope.hpp"
    #include "di/front/base/aux/bind.hpp"
    #include "di/front/base/aux/externals.hpp"
    #include "di/front/base/aux/annotate.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_MPL_LIMIT_VECTOR_SIZE, "di/front/base/module.hpp"))

    namespace di
    {
    namespace front
    {
    namespace base
    {

    template<typename TScope> struct Scope : aux::Scope<TScope> { };
    template<typename TExpected, typename TGiven = TExpected> struct Bind : aux::Bind<TExpected, TGiven> { };

    template<typename TExpected, typename TGiven = TExpected> struct Singleton : Scope<back::Scopes::Singleton>::Bind< Bind<TExpected, TGiven> > { };
    template<typename T> struct Singleton<T, T> : Scope<back::Scopes::Singleton>::Bind<T> { };

    template<typename TExpected, typename TGiven = TExpected> struct PerRequest : Scope<back::Scopes::PerRequest>::Bind< Bind<TExpected, TGiven> > { };
    template<typename T> struct PerRequest<T, T> : Scope<back::Scopes::PerRequest>::Bind<T> { };

    template<typename T> struct External : aux::Externals<T> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct Singletons : Scope<back::Scopes::Singleton>::Bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct PerRequests : Scope<back::Scopes::PerRequest>::Bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct Externals : aux::Externals<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template<typename T> struct Annotate : aux::Annotate<T> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    class Module : public back::Module
    {
        template<typename TInstance, typename T> struct IsSameInstance : boost::mpl::or_
            <
                boost::is_same<typename TInstance::Name, T>,
                boost::is_same<typename TInstance::valueType, T>
            >
        { };

        template<typename TSeq, typename T> struct FindInstanceType
            : boost::mpl::find_if<TSeq, IsSameInstance<boost::mpl::_1, T> >::type
        { };

        template<typename T, typename Enable = void>
        struct MakeAnnotation
        {
            typedef typename Annotate<back::aux::Instance<T> >::template With<> type;
        };

        template<typename T>
        struct MakeAnnotation<T, typename boost::enable_if<boost::is_base_of<base::aux::Internal, T> >::type>
        {
            typedef typename T::template Rebind<back::Scopes::Singleton>::type Dependency;
            typedef back::aux::Instance<typename Dependency::Expected, typename Dependency::Context> Instance;
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
                            boost::is_base_of<aux::detail::Externals, boost::mpl::_2>,
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
                back::aux::get_derived<boost::mpl::_1>
            >::type
        { };

    public:
        typedef back::aux::Pool<typename Instances::type> Pool;

        struct Dependencies : boost::mpl::fold
            <
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                boost::mpl::vector0<>,
                boost::mpl::copy
                <
                    boost::mpl::if_
                    <
                        boost::is_base_of<aux::detail::Externals, boost::mpl::_2>,
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

        template<typename T, typename Tvalue>
        inline static typename boost::disable_if<boost::is_same<FindInstanceType<Externals, T>, boost::mpl::end<Externals> >, typename FindInstanceType<Externals, T>::type::Derived>::type
        Set(Tvalue p_value)
        {
            typedef typename FindInstanceType<Externals, T>::type Annotation;
            return typename Annotation::Derived(p_value);
        }

        const Pool& pool() const { return m_pool; }

    private:
        Pool m_pool;
    };

    } // namespace base
    } // namespace front
    } // namespace di

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    Module(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg))
        : m_pool(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg))
    { }

#endif

