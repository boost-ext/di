//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef DI_BACK_DETAIL_VISITOR_HPP
    #define DI_BACK_DETAIL_VISITOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include "di/back/aux/utility.hpp"
    #include "di/back/aux/dependency.hpp"
    #include "di/back/detail/binder.hpp"
    #include "di/back/scopes/per_request.hpp"
    #include "di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, DI_FUNCTION_ARITY_LIMIT_SIZE, "di/back/detail/visitor.hpp"))

    namespace di
    {
    namespace back
    {
    namespace detail
    {

    template
    <
        typename TDeps,
        template<typename, typename, typename = TDeps, typename = aux::Dependency<Scopes::PerRequest, boost::mpl::_1, boost::mpl::_2> > class TBinder = Binder
    >
    class VisitorImpl
    {
        template<typename T, typename TCallStack, typename TDependency>
        struct Dependency
        {
            typedef T Type;
            typedef TCallStack Context;
            typedef typename TDependency::Given Given;
            typedef typename TDependency::Expected Expected;
        };

    public:
        template<typename T, typename TCallStack, typename TVisitor>
        static void execute(const TVisitor& p_visitor)
        {
            typedef typename TBinder<T, TCallStack>::type ToBeCreated;
            typedef typename aux::update_call_stack<TCallStack, ToBeCreated>::type CallStack;
            executeImpl<T, ToBeCreated, CallStack>(p_visitor);
        }

    private:
        #include BOOST_PP_ITERATE()
    };

    template<typename TDeps> struct Visitor : VisitorImpl<TDeps> { };

    } // namespace detail
    } // namespace back
    } // namespace di

    #endif

#else

    template<typename T, typename TDependency, typename TCallStack, typename TVisitor>
    static typename aux::enable_if_ctor_size<TDependency, BOOST_PP_ITERATION()>::type executeImpl(const TVisitor& p_visitor)
    {
        p_visitor.template operator()< Dependency<T, TCallStack, TDependency> >();

        #define DI_VISITOR_EXECUTE(z, n, _)\
            execute<typename aux::at_ctor<TDependency, n>::type, TCallStack>(p_visitor);

        BOOST_PP_REPEAT(BOOST_PP_ITERATION(), DI_VISITOR_EXECUTE, ~);

        #undef DI_VISITOR_EXECUTE
    }

#endif

