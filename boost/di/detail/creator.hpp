//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_CREATOR_HPP
    #define BOOST_DI_DETAIL_CREATOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include "boost/di/aux/utility.hpp"
    #include "boost/di/aux/dependency.hpp"
    #include "boost/di/detail/binder.hpp"
    #include "boost/di/detail/converter.hpp"
    #include "boost/di/scopes/per_request.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE, "boost/di/detail/creator.hpp"))

    namespace boost {
    namespace di {
    namespace detail {

    template
    <
        typename TDeps,
        typename TPool,
        template<typename, typename, typename = TDeps, typename = aux::dependency<scopes::per_request, mpl::_1, mpl::_2> > class TBinder = binder,
        template<typename, typename> class TConverter = converter
    >
    class creator_impl
    {
    public:
        template<typename T, typename TCallStack, typename TEntries>
        static typename TBinder<T, TCallStack>::type::template result_type<TPool>::type execute(TEntries& entries, const TPool& pool)
        {
            typedef typename TBinder<T, TCallStack>::type ToBeCreated;
            typedef typename aux::update_call_stack<TCallStack, ToBeCreated>::type CallStack;
            return execute_impl<ToBeCreated, CallStack, TEntries>(entries, pool);
        }

    private:
        #include BOOST_PP_ITERATE()

        template<typename TDependency, typename TEntries>
        static typename enable_if<is_base_of<TDependency, TEntries>, TDependency&>::type acquire(TEntries& entries)
        {
            return static_cast<TDependency&>(entries);
        }

        template<typename TDependency, typename TEntries>
        static typename disable_if<is_base_of<TDependency, TEntries>, TDependency>::type acquire(TEntries&)
        {
            return TDependency();
        }
    };

    template<typename TDeps, typename TPool>
    struct creator
        : creator_impl<TDeps, TPool>
    { };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename TDependency, typename TCallStack, typename TEntries>
    static typename aux::enable_if_ctor_size<TDependency, BOOST_PP_ITERATION(), typename TDependency::template result_type<TPool>::type>::type
    execute_impl(TEntries& entries, const TPool& pool)
    {
        #define BOOST_DI_CREATOR_EXECUTE(z, n, _) BOOST_PP_COMMA_IF(n)                                         \
             TConverter<typename TDependency::scope, typename aux::at_ctor<TDependency, n>::type>::execute(  \
                execute<typename aux::at_ctor<TDependency, n>::type, TCallStack>(entries, pool))

        return acquire<TDependency>(entries).create(
            pool BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_CREATOR_EXECUTE, ~));

        #undef BOOST_DI_CREATOR_EXECUTE
    }

#endif

