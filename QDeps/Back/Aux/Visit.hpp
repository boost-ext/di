//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_BACK_AUX_VISIT_HPP
    #define QDEPS_BACK_AUX_VISIT_HPP

    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/preprocessor/control/expr_if.hpp>
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
    #include "QDeps/Back/Aux/Utility.hpp"
    #include "QDeps/Back/Aux/Binding.hpp"
    #include "QDeps/Back/Scopes/PerRequest.hpp"
    #include "QDeps/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, QDEPS_FUNCTION_ARITY_LIMIT_SIZE, "QDeps/Back/Aux/Visit.hpp"))

    namespace QDeps
    {
    namespace Back
    {
    namespace Aux
    {

    template
    <
        typename TDeps,
        template<typename, typename, typename = TDeps> class TBinding = Binding
    >
    class Visit
    {
    public:
        template<typename T, typename TCallStack, typename TVisitor> static void execute
        (
            const TVisitor& p_visitor,
            typename boost::enable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::enable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
        )
        {
            typedef typename boost::function_types::parameter_types<typename GetCtor<T>::type>::type Ctor;
            typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
            execute<T, Ctor, CallStack, Scopes::PerRequest>(p_visitor);
        }

        template<typename T, typename TCallStack, typename TVisitor> static void execute
        (
            const TVisitor& p_visitor,
            typename boost::enable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::disable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
        )
        {
            typedef typename boost::function_types::parameter_types<typename GetCtor<T>::type>::type Ctor;
            typedef typename TBinding<T, TCallStack>::type Binding;
            typedef typename boost::mpl::deref<typename boost::mpl::begin<Binding>::type>::type ToBeCreated;
            typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
            execute<T, Ctor, CallStack, typename ToBeCreated::Scope>(p_visitor);
        }

        template<typename T, typename TCallStack, typename TVisitor> static void execute
        (
            const TVisitor& p_visitor,
            typename boost::disable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::enable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
        )
        {
            p_visitor.template operator()<T, TCallStack, boost::mpl::_1>();
        }

        template<typename T, typename TCallStack, typename TVisitor> static void execute
        (
            const TVisitor& p_visitor,
            typename boost::disable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::disable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
        )
        {
            typedef typename boost::mpl::vector0<> Ctor;
            typedef typename TBinding<T, TCallStack>::type Binding;
            typedef typename boost::mpl::deref<typename boost::mpl::begin<Binding>::type>::type ToBeCreated;
            typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
            execute<T, Ctor, CallStack, typename ToBeCreated::Scope>(p_visitor);
        }

        #include BOOST_PP_ITERATE()
    };

    template
    <
        typename TDeps
    >
    struct Visit2
    {
        typedef Visit<TDeps> type;
    };

    } // namespace Aux
    } // namespace Back
    } // namespace QDeps

    #endif

#else

    template<typename T, typename TCtor, typename TCallStack, typename TScope, typename TVisitor> static void execute
    (
        const TVisitor& p_visitor,
        typename boost::enable_if_c<boost::mpl::size<TCtor>::value == BOOST_PP_ITERATION()>::type* = 0
    )
    {
        p_visitor.template operator()<T, TCallStack, TScope>();

/*        typedef typename MakePlain<typename boost::mpl::at_c<TCtor, n>::type>::type, TCallStack>*/

        /*execute<typename MakePlain<typename boost::mpl::at_c<Ctor, n>::type>::type, TCallStack>(*/
    }

#endif

