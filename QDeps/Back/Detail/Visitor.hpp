//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_BACK_DETAIL_VISITOR_HPP
    #define QDEPS_BACK_DETAIL_VISITOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include "QDeps/Back/Aux/Utility.hpp"
    #include "QDeps/Back/Aux/Dependency.hpp"
    #include "QDeps/Back/Detail/Binder.hpp"
    #include "QDeps/Back/Scopes/PerRequest.hpp"
    #include "QDeps/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, QDEPS_FUNCTION_ARITY_LIMIT_SIZE, "QDeps/Back/Detail/Visitor.hpp"))

    namespace QDeps
    {
    namespace Back
    {
    namespace Detail
    {

    template
    <
        typename TDeps,
        template<typename, typename, typename = TDeps, typename = Aux::Dependency<Scopes::PerRequest, boost::mpl::_1, boost::mpl::_2> > class TBinder = Binder
    >
    class VisitorImpl
    {
        template<typename T, typename TDependency>
        struct Dependency
        {
            typedef T Type;
            typedef typename TDependency::Given Given;
            typedef typename TDependency::Expected Expected;
            typedef typename TDependency::Context Context;
        };

    public:
        template<typename T, typename TCallStack, typename TVisitor>
        static void execute(const TVisitor& p_visitor)
        {
            typedef typename TBinder<T, TCallStack>::type ToBeCreated;
            typedef typename Aux::UpdateCallStack<TCallStack, ToBeCreated>::type CallStack;
            executeImpl<T, ToBeCreated, CallStack>(p_visitor);
        }

    private:
        #include BOOST_PP_ITERATE()
    };

    template<typename TDeps> struct Visitor : VisitorImpl<TDeps> { };

    } // namespace Detail
    } // namespace Back
    } // namespace QDeps

    #endif

#else

    template<typename T, typename TDependency, typename TCallStack, typename TVisitor>
    static typename Aux::EnableIfCtorSize<TDependency, BOOST_PP_ITERATION()>::type executeImpl(const TVisitor& p_visitor)
    {
        p_visitor.template operator()< Dependency<T, TDependency> >();

        #define QDEPS_VISITOR_EXECUTE(z, n, _)\
            execute<typename Aux::AtCtor<TDependency, n>::type, TCallStack>(p_visitor);

        BOOST_PP_REPEAT(BOOST_PP_ITERATION(), QDEPS_VISITOR_EXECUTE, ~);

        #undef QDEPS_VISITOR_EXECUTE
    }

#endif

