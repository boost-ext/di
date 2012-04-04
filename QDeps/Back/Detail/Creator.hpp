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
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
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
        template<typename, typename, typename = TDeps, typename = Aux::Dependency<Scopes::PerRequest, boost::mpl::_1> > class TBinder = Binder,
        template<typename> class TConverter = Converter
    >
    class Creator
    {
    public:
        template<typename T, typename TCallStack, typename TEntries>
        static typename TBinder<T, TCallStack>::type::template ResultType<TPool>::type execute(TEntries& p_entries, TPool& p_pool)
        {
            typedef typename TBinder<T, TCallStack>::type ToBeCreated;
            typedef typename boost::mpl::push_back<TCallStack, typename ToBeCreated::Given>::type CallStack;
            return execute<T, CallStack, ToBeCreated, TEntries>(p_entries, p_pool);
        }

        template<typename T, typename TCallStack, typename TDependency, typename TEntries>
        static typename TDependency::template ResultType<TPool>::type execute
        (
            TEntries&, TPool&,
            typename boost::disable_if<Aux::IsUniqueCallStack<TCallStack> >::type* = 0
        )
        {
            QDEPS_STATIC_ASSERT(
                false,
                CIRCULAR_DEPENDENCIES_NOT_ALLOWED,
                (T, TCallStack)
            );
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

    template<typename T, typename TCallStack, typename TDependency, typename TEntries>
    static typename TDependency::template ResultType<TPool>::type execute
    (
        TEntries& p_entries, TPool& p_pool,
        typename boost::enable_if_c<boost::mpl::size<typename TDependency::Ctor>::value == BOOST_PP_ITERATION()>::type* = 0,
        typename boost::enable_if<Aux::IsUniqueCallStack<TCallStack> >::type* = 0
    )
    {
        #define QDEPS_CREATOR_IMPL_ARG(z, n, text) BOOST_PP_COMMA_IF(n)                                             \
             TConverter<typename boost::mpl::at_c<typename TDependency::Ctor, n>::type>::execute(                   \
                execute                                                                                             \
                <                                                                                                   \
                    typename Aux::MakePlain<typename boost::mpl::at_c<typename TDependency::Ctor, n>::type>::type,  \
                    TCallStack                                                                                      \
                >                                                                                                   \
                (p_entries, p_pool)                                                                                 \
             )

        return static_cast<TDependency&>(p_entries).create(
            p_pool BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_REPEAT(BOOST_PP_ITERATION(), QDEPS_CREATOR_IMPL_ARG, ~));

        #undef QDEPS_CREATOR_IMPL_ARG
    }

#endif

