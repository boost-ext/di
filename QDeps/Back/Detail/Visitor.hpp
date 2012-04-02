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
    #include <boost/preprocessor/cat.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/function_types/parameter_types.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include <boost/mpl/deref.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include <boost/mpl/is_sequence.hpp>
    #include "QDeps/Back/Aux/Utility.hpp"
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

    namespace Detail
    {

    template
    <
        typename TDeps,
        template<typename, typename, typename = TDeps> class TBinder = Binder
    >
    class Visitor
    {
    public:
        template<typename T, typename TGiven, typename TCallStack, typename TVisitor> static void execute
        (
            const TVisitor& p_visitor,
            typename boost::enable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::enable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type* = 0
        )
        {
            typedef typename boost::function_types::parameter_types<typename Aux::GetCtor<T>::type>::type Ctor;
            typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
            execute<T, TGiven, Ctor, CallStack, Scopes::PerRequest>(p_visitor);
        }

        template<typename T, typename TGiven, typename TCallStack, typename TVisitor> static void execute
        (
            const TVisitor& p_visitor,
            typename boost::enable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::disable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type* = 0
        )
        {
            typedef typename boost::function_types::parameter_types<typename Aux::GetCtor<T>::type>::type Ctor;
            typedef typename TBinder<T, TCallStack>::type Binding;
            typedef typename boost::mpl::deref<typename boost::mpl::begin<Binding>::type>::type ToBeCreated;
            typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
            execute<T, TGiven, Ctor, CallStack, typename ToBeCreated::Scope>(p_visitor);
        }

        template<typename T, typename TGiven, typename TCallStack, typename TVisitor> static void execute
        (
            const TVisitor& p_visitor,
            typename boost::disable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::enable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type* = 0
        )
        {
            p_visitor.template operator()<TGiven, typename boost::mpl::push_back<TCallStack, T>::type, boost::mpl::_1>();
        }

        template<typename T, typename TGiven, typename TCallStack, typename TVisitor> static void execute
        (
            const TVisitor& p_visitor,
            typename boost::disable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::disable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type* = 0
        )
        {
            typedef typename boost::mpl::vector0<> Ctor;
            typedef typename TBinder<T, TCallStack>::type Binding;
            typedef typename boost::mpl::deref<typename boost::mpl::begin<Binding>::type>::type ToBeCreated;
            typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
            execute<T, TGiven, Ctor, CallStack, typename ToBeCreated::Scope>(p_visitor);
        }

        #include BOOST_PP_ITERATE()

        template<typename T, typename TGiven, typename TCtor, typename TCallStack, typename TScope, typename TVisitor>
        static typename boost::disable_if<Aux::IsUniqueCallStack<TCallStack> >::type execute(const TVisitor&)
        {
            QDEPS_STATIC_ASSERT(
                false,
                CIRCULAR_DEPENDENCIES_NOT_ALLOWED,
                (T, TCallStack)
            );
        }
    };

    } // namespace Detail

    template<typename TDeps>
    struct Visitor : Detail::Visitor<TDeps>
    { };

    } // namespace Detail
    } // namespace Back
    } // namespace QDeps

    #endif

#else

    template<typename T, typename TGiven, typename TCtor, typename TCallStack, typename TScope, typename TVisitor>
    static typename boost::enable_if<boost::mpl::is_sequence<TCtor> >::type execute
    (
        const TVisitor& p_visitor,
        typename boost::enable_if_c<boost::mpl::size<TCtor>::value == BOOST_PP_ITERATION()>::type* = 0,
        typename boost::enable_if<Aux::IsUniqueCallStack<TCallStack> >::type* = 0
    )
    {
        p_visitor.template operator()<TGiven, TCallStack, TScope>();

        #define QDEPS_EXECUTE(z, n, text)\
            execute<typename Aux::MakePlain<typename boost::mpl::at_c<TCtor, n>::type>::type, typename boost::mpl::at_c<TCtor, n>::type, TCallStack>(p_visitor);

        BOOST_PP_REPEAT(BOOST_PP_ITERATION(), QDEPS_EXECUTE, ~);

        #undef QDEPS_EXECUTE
    }

#endif

