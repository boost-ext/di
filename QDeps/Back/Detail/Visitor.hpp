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
    #include <boost/utility/enable_if.hpp>
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
        template<typename, typename, typename = TDeps, typename = Aux::Dependency<Scopes::PerRequest, boost::mpl::_1> > class TBinder = Binder
    >
    class VisitorImpl
    {
    public:
        template<typename T, typename TGiven, typename TCallStack, typename TVisitor>
        static void execute(const TVisitor& p_visitor)
        {
            typedef typename TBinder<T, TCallStack>::type ToBeCreated;
            typedef typename boost::mpl::push_back<TCallStack, typename ToBeCreated::Given>::type CallStack;
            execute<T, TGiven, CallStack, ToBeCreated>(p_visitor);
        }

        #include BOOST_PP_ITERATE()
    };

    template<typename TDeps> struct Visitor : VisitorImpl<TDeps> { };

    } // namespace Detail
    } // namespace Back
    } // namespace QDeps

    #endif

#else

    template<typename T, typename TGiven, typename TCallStack, typename TDependency, typename TVisitor>
    static void execute
    (
        const TVisitor& p_visitor,
        typename boost::enable_if_c<boost::mpl::size<typename TDependency::Ctor>::value == BOOST_PP_ITERATION()>::type* = 0
    )
    {
        p_visitor.template operator()<TGiven, TCallStack, typename TDependency::Scope>();

        #define QDEPS_EXECUTE(z, n, text)                                                                       \
            execute                                                                                             \
            <                                                                                                   \
                typename Aux::MakePlain<typename boost::mpl::at_c<typename TDependency::Ctor, n>::type>::type,  \
                typename boost::mpl::at_c<typename TDependency::Ctor, n>::type, TCallStack                      \
            >                                                                                                   \
            (p_visitor);

        BOOST_PP_REPEAT(BOOST_PP_ITERATION(), QDEPS_EXECUTE, ~);

        #undef QDEPS_EXECUTE
    }

#endif

