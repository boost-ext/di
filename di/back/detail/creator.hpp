//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef DI_BACK_DETAIL_CREATOR_HPP
    #define DI_BACK_DETAIL_CREATOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include "di/back/aux/utility.hpp"
    #include "di/back/aux/dependency.hpp"
    #include "di/back/detail/binder.hpp"
    #include "di/back/detail/converter.hpp"
    #include "di/back/scopes/per_request.hpp"
    #include "di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, DI_FUNCTION_ARITY_LIMIT_SIZE, "di/back/detail/creator.hpp"))

    namespace di
    {
    namespace back
    {
    namespace detail
    {

    template
    <
        typename TDeps,
        typename TPool,
        template<typename, typename, typename = TDeps, typename = aux::dependency<scopes::per_request, boost::mpl::_1, boost::mpl::_2> > class TBinder = binder,
        template<typename, typename> class Tconverter = converter
    >
    class creator_impl
    {
    public:
        template<typename T, typename TCallStack, typename TEntries>
        static typename TBinder<T, TCallStack>::type::template result_type<TPool>::type execute(TEntries& p_entries, const TPool& p_pool)
        {
            typedef typename TBinder<T, TCallStack>::type ToBeCreated;
            typedef typename aux::update_call_stack<TCallStack, ToBeCreated>::type CallStack;
            return execute_impl<ToBeCreated, CallStack, TEntries>(p_entries, p_pool);
        }

    private:
        #include BOOST_PP_ITERATE()

        template<typename TDependency, typename TEntries>
        static typename boost::enable_if<boost::is_base_of<TDependency, TEntries>, TDependency&>::type acquire(TEntries& p_entries)
        {
            return static_cast<TDependency&>(p_entries);
        }

        template<typename TDependency, typename TEntries>
        static typename boost::disable_if<boost::is_base_of<TDependency, TEntries>, TDependency>::type acquire(TEntries&)
        {
            return TDependency();
        }
    };

    template<typename TDeps, typename TPool> struct creator : creator_impl<TDeps, TPool> { };

    } // namespace detail
    } // namespace back
    } // namespace di

    #endif

#else

    template<typename TDependency, typename TCallStack, typename TEntries>
    static typename aux::enable_if_ctor_size<TDependency, BOOST_PP_ITERATION(), typename TDependency::template result_type<TPool>::type>::type
    execute_impl(TEntries& p_entries, const TPool& p_pool)
    {
        #define DI_CREATOR_EXECUTE(z, n, _) BOOST_PP_COMMA_IF(n)                                         \
             Tconverter<typename TDependency::scope, typename aux::at_ctor<TDependency, n>::type>::execute(  \
                execute<typename aux::at_ctor<TDependency, n>::type, TCallStack>(p_entries, p_pool))

        return acquire<TDependency>(p_entries).create(
            p_pool BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_REPEAT(BOOST_PP_ITERATION(), DI_CREATOR_EXECUTE, ~));

        #undef DI_CREATOR_EXECUTE
    }

#endif

