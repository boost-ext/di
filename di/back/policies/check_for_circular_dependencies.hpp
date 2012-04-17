//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING


    #ifndef DI_BACK_POLICIES_CHECK_FOR_CIRCULAR_DEPENDENCIES_HPP
    #define DI_BACK_POLICIES_CHECK_FOR_CIRCULAR_DEPENDENCIES_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/control/expr_if.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/count.hpp>
    #include <boost/mpl/int.hpp>
    #include <boost/mpl/plus.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/accumulate.hpp>
    #include <boost/mpl/transform.hpp>
    #include <boost/mpl/size.hpp>
    #include "di/back/aux/utility.hpp"
    #include "di/back/aux/dependency.hpp"
    #include "di/back/detail/binder.hpp"
    #include "di/back/scopes/per_request.hpp"
    #include "di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, DI_FUNCTION_ARITY_LIMIT_SIZE, "di/back/policies/check_for_circular_dependencies.hpp"))

    namespace di
    {
    namespace Back
    {
    namespace Policies
    {

    class CheckForCircularDependencies
    {
    public:
        template<typename TDeps, typename TGiven, template<typename, typename, typename = TDeps, typename = Aux::Dependency<Scopes::PerRequest, boost::mpl::_1, boost::mpl::_2> > class TBinder = Detail::Binder>
        class Assert
        {
            template<typename TCallStack>
            struct IsUniqueCallStack : boost::mpl::bool_
                <
                    static_cast<std::size_t>(boost::mpl::accumulate
                        <
                            typename boost::mpl::transform<TCallStack, boost::mpl::count<TCallStack, boost::mpl::_> >::type,
                            boost::mpl::int_<0>,
                            boost::mpl::plus< boost::mpl::_1, boost::mpl::_2>
                        >::type::value
                    ) == boost::mpl::size<TCallStack>::value
                >
            { };

            template<typename, typename, typename = void, typename = void> struct CircularDependenciesImpl;

            template<typename T, typename TCallStack, typename = void, typename = void>
            struct CircularDependencies : CircularDependenciesImpl
                <
                    typename TBinder<T, TCallStack>::type,
                    typename Aux::UpdateCallStack<TCallStack, typename TBinder<T, TCallStack>::type>::type
                >
            { };

            #include BOOST_PP_ITERATE()

        public:
            typedef CircularDependencies<TGiven, boost::mpl::vector0<> > type;
        };
    };

    } // namespace Policies
    } // namespace Back
    } // namespace di

    #endif

#else

    #define DI_CHECK_FOR_CIRCULAR_DEPENDENCIES_IMPL(z, n, _) BOOST_PP_COMMA_IF(n)\
        CircularDependencies<typename Aux::AtCtor<TDependency, n>::type, TCallStack>

    template<typename TDependency, typename TCallStack>
    struct CircularDependenciesImpl
        <
            TDependency,
            TCallStack,
            typename Aux::EnableIfCtorSize<TDependency, BOOST_PP_ITERATION()>::type,
            typename boost::enable_if< IsUniqueCallStack<TCallStack> >::type
        >
    BOOST_PP_EXPR_IF(BOOST_PP_ITERATION(), :)
        BOOST_PP_REPEAT(BOOST_PP_ITERATION(), DI_CHECK_FOR_CIRCULAR_DEPENDENCIES_IMPL, ~)
    { };

    template<typename TDependency, typename TCallStack>
    struct CircularDependenciesImpl
        <
            TDependency,
            TCallStack,
            typename Aux::EnableIfCtorSize<TDependency, BOOST_PP_ITERATION()>::type,
            typename boost::disable_if< IsUniqueCallStack<TCallStack> >::type
        >
    :
        boost::mpl::false_
    {
       DI_STATIC_ASSERT(
            false,
            CIRCULAR_DEPENDENCIES_ARE_NOT_ALLOWED,
            (typename TDependency::Given, TCallStack)
        );
    };

    #undef DI_CHECK_FOR_CIRCULAR_DEPENDENCIES_IMPL

#endif

