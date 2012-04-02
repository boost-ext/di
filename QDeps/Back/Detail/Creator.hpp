//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_BACK_DETAIL_CREATOR_HPP
    #define QDEPS_BACK_DETAIL_CREATOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/preprocessor/control/expr_if.hpp>
    #include <boost/preprocessor/cat.hpp>
    #include <boost/make_shared.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/function_types/parameter_types.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include <boost/mpl/deref.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/assert.hpp>
    #include "QDeps/Back/Aux/Utility.hpp"
    #include "QDeps/Back/Aux/Dependency.hpp"
    #include "QDeps/Back/Detail/Binder.hpp"
    #include "QDeps/Back/Detail/Converter.hpp"
    #include "QDeps/Back/Scopes/PerRequest.hpp"
    #include "QDeps/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, QDEPS_FUNCTION_ARITY_LIMIT_SIZE, "QDeps/Back/Detail/Creator.hpp"))

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
        typename TPool,
        template<typename, typename, typename = TDeps> class TBinder = Binder,
        template<typename> class TConverter = Converter
        //TODO Aux::Dependency
    >
    class Creator
    {
        template<typename, typename, typename, typename = void, typename = void> class CreatorImpl;

        template<typename T, typename TCallStack, typename TEntries> class CreatorImpl
        <
            T, TCallStack, TEntries,
            typename boost::enable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type,
            typename boost::enable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type
        >
        {
        public:
            typedef typename boost::function_types::parameter_types<typename Aux::GetCtor<T>::type>::type Ctor;
            typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
            typedef typename TBinder<T, TCallStack>::type Binding;
            typedef typename Aux::Dependency<Scopes::PerRequest, T>::template ResultType<TPool>::type ResultOf;

            static ResultOf execute(TEntries& p_entries, TPool& p_pool)
            {
                Aux::Dependency<Scopes::PerRequest, T> l_onDemandInst;
                return Creator::template execute<T, Ctor, CallStack>(p_entries, l_onDemandInst, p_pool);
            }
        };

        template<typename T, typename TCallStack, typename TEntries> class CreatorImpl
        <
            T, TCallStack, TEntries,
            typename boost::enable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type,
            typename boost::disable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type
        >
        {
        public:
            typedef typename boost::function_types::parameter_types<typename Aux::GetCtor<T>::type>::type Ctor;
            typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
            typedef typename TBinder<T, TCallStack>::type Binding;
            typedef typename boost::mpl::deref<typename boost::mpl::begin<Binding>::type>::type ToBeCreatord;
            typedef typename ToBeCreatord::template ResultType<TPool>::type ResultOf;

            static ResultOf execute(TEntries& p_entries, TPool& p_pool)
            {
                return Creator::template execute<T, Ctor, CallStack>(p_entries, static_cast<ToBeCreatord&>(p_entries), p_pool);
            }
        };

        template<typename T, typename TCallStack, typename TEntries> class CreatorImpl
        <
            T, TCallStack, TEntries,
            typename boost::disable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type,
            typename boost::enable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type
        >
        {
        public:
            typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
            typedef typename TBinder<T, TCallStack>::type Binding;
            typedef boost::shared_ptr<T> ResultOf;

            static ResultOf execute(TEntries&, TPool&)
            {
                return boost::make_shared<T>();
            }
        };

        template<typename T, typename TCallStack, typename TEntries> class CreatorImpl
        <
            T, TCallStack, TEntries,
            typename boost::disable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type,
            typename boost::disable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type
        >
        {
        public:
            typedef boost::mpl::vector0<> Ctor;
            typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
            typedef typename TBinder<T, TCallStack>::type Binding;
            typedef typename boost::mpl::deref<typename boost::mpl::begin<Binding>::type>::type ToBeCreatord;
            typedef typename ToBeCreatord::template ResultType<TPool>::type ResultOf;

            static ResultOf execute(TEntries& p_entries, TPool& p_pool)
            {
                return Creator::template execute<T, Ctor, CallStack>(p_entries, static_cast<ToBeCreatord&>(p_entries), p_pool);
            }
        };

    public:
        template<typename T, typename TCallStack, typename TEntries>
        static typename CreatorImpl<T, TCallStack, TEntries>::ResultOf execute
        (
            TEntries& p_entries,
            TPool& p_pool,
            typename boost::enable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::enable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type* = 0
        )
        {
            return CreatorImpl<T, TCallStack, TEntries>::execute(p_entries, p_pool);
        }

        template<typename T, typename TCallStack, typename TEntries>
        static typename CreatorImpl<T, TCallStack, TEntries>::ResultOf execute
        (
            TEntries& p_entries,
            TPool& p_pool,
            typename boost::enable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::disable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type* = 0
        )
        {
            return CreatorImpl<T, TCallStack, TEntries>::execute(p_entries, p_pool);
        }

        template<typename T, typename TCallStack, typename TEntries>
        static typename CreatorImpl<T, TCallStack, TEntries>::ResultOf execute
        (
            TEntries& p_entries,
            TPool& p_pool,
            typename boost::disable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::disable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type* = 0
        )
        {
            return CreatorImpl<T, TCallStack, TEntries>::execute(p_entries, p_pool);
        }

        template<typename T, typename TCallStack, typename TEntries>
        static typename CreatorImpl<T, TCallStack, TEntries>::ResultOf execute
        (
            TEntries& p_entries,
            TPool& p_pool,
            typename boost::disable_if< BOOST_PP_CAT(Aux::Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
            typename boost::enable_if< boost::mpl::empty<typename TBinder<T, TCallStack>::type> >::type* = 0
        )
        {
            return CreatorImpl<T, TCallStack, TEntries>::execute(p_entries, p_pool);
        }

        #include BOOST_PP_ITERATE()
    };

    } // namespace Detail

    template<typename TDeps, typename TPool>
    struct Creator : Detail::Creator<TDeps, TPool>
    { };

    } // namespace Detail
    } // namespace Back
    } // namespace QDeps

    #endif

#else

    template
    <
        typename T,
        typename TCtor,
        typename TCallStack,
        typename TEntries,
        typename TDependency
    >
    static typename TDependency::template ResultType<TPool>::type execute
    (
        TEntries& BOOST_PP_EXPR_IF(BOOST_PP_ITERATION(), p_entries),
        TDependency& p_dependency,
        TPool& p_pool,
        typename boost::enable_if_c<boost::mpl::size<TCtor>::value == BOOST_PP_ITERATION()>::type* = 0,
        typename boost::enable_if<Aux::IsUniqueCallStack<TCallStack> >::type* = 0
    )
    {
        #define QDEPS_CREATOR_IMPL_ARG(z, n, text) BOOST_PP_COMMA_IF(n)                                                             \
             TConverter<typename boost::mpl::at_c<TCtor, n>::type>::execute(                                                        \
                execute<typename Aux::MakePlain<typename boost::mpl::at_c<TCtor, n>::type>::type, TCallStack>(p_entries, p_pool)    \
             )

        return p_dependency.create(
            p_pool BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_REPEAT(BOOST_PP_ITERATION(), QDEPS_CREATOR_IMPL_ARG, ~));

        #undef QDEPS_CREATOR_IMPL_ARG
    }

#endif

