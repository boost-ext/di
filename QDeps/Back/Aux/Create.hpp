//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_CREATE_HPP
#define QDEPS_BACK_AUX_CREATE_HPP

#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/make_shared.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/has_xxx.hpp>
#include "QDeps/Back/Aux/Utility.hpp"
#include "QDeps/Back/Aux/Dependency.hpp"
#include "QDeps/Back/Aux/Binding.hpp"
#include "QDeps/Back/Scopes/PerRequest.hpp"

//TODO iterate

namespace QDeps
{
namespace Back
{
namespace Aux
{

namespace Detail
{

template
<
    typename TDeps,
    typename TPool,
    template<typename, typename, typename = TDeps> class TBinding = Binding
    //TODO Dependency
>
class Create
{
    template<typename, typename, typename, typename = void, typename = void> class CreateImpl;

    template<typename T, typename TCallStack, typename TEntries> class CreateImpl
    <
        T, TCallStack, TEntries,
        typename boost::enable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type,
        typename boost::enable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type
    >
    {
    public:
        typedef typename boost::function_types::parameter_types<typename GetCtor<T>::type>::type Ctor;
        typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
        typedef typename TBinding<T, TCallStack>::type Binding;
        typedef typename Dependency<Scopes::PerRequest, T>::template ResultType<TPool>::type ResultOf;

        static ResultOf execute(TEntries& p_entries, TPool& p_pool)
        {
            Dependency<Scopes::PerRequest, T> l_onDemandInst;
            return Create::template execute<T, Ctor, CallStack>(p_entries, l_onDemandInst, p_pool);
        }
    };

    template<typename T, typename TCallStack, typename TEntries> class CreateImpl
    <
        T, TCallStack, TEntries,
        typename boost::enable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type,
        typename boost::disable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type
    >
    {
    public:
        typedef typename boost::function_types::parameter_types<typename GetCtor<T>::type>::type Ctor;
        typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
        typedef typename TBinding<T, TCallStack>::type Binding;
        typedef typename boost::mpl::deref<typename boost::mpl::begin<Binding>::type>::type ToBeCreated;
        typedef typename ToBeCreated::template ResultType<TPool>::type ResultOf;

        static ResultOf execute(TEntries& p_entries, TPool& p_pool)
        {
            return Create::template execute<T, Ctor, CallStack>(p_entries, static_cast<ToBeCreated&>(p_entries), p_pool);
        }
    };

    template<typename T, typename TCallStack, typename TEntries> class CreateImpl
    <
        T, TCallStack, TEntries,
        typename boost::disable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type,
        typename boost::enable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type
    >
    {
    public:
        typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
        typedef typename TBinding<T, TCallStack>::type Binding;
        typedef boost::shared_ptr<T>/*T*/ ResultOf;

        static ResultOf execute(TEntries&, TPool&)
        {
            //return T();
            return boost::make_shared<T>();
        }
    };

    template<typename T, typename TCallStack, typename TEntries> class CreateImpl
    <
        T, TCallStack, TEntries,
        typename boost::disable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type,
        typename boost::disable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type
    >
    {
    public:
        typedef boost::mpl::vector0<> Ctor;
        typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
        typedef typename TBinding<T, TCallStack>::type Binding;
        typedef typename boost::mpl::deref<typename boost::mpl::begin<Binding>::type>::type ToBeCreated;
        typedef typename ToBeCreated::template ResultType<TPool>::type ResultOf;

        static ResultOf execute(TEntries& p_entries, TPool& p_pool)
        {
            return Create::template execute<T, Ctor, CallStack>(p_entries, static_cast<ToBeCreated&>(p_entries), p_pool);
        }
    };

public:
    template<typename T, typename TCallStack, typename TEntries>
    static typename CreateImpl<T, TCallStack, TEntries>::ResultOf execute
    (
        TEntries& p_entries,
        TPool& p_pool,
        typename boost::enable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::enable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
    )
    {
        return CreateImpl<T, TCallStack, TEntries>::execute(p_entries, p_pool);
    }

    template<typename T, typename TCallStack, typename TEntries>
    static typename CreateImpl<T, TCallStack, TEntries>::ResultOf execute
    (
        TEntries& p_entries,
        TPool& p_pool,
        typename boost::enable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::disable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
    )
    {
        return CreateImpl<T, TCallStack, TEntries>::execute(p_entries, p_pool);
    }

    template<typename T, typename TCallStack, typename TEntries>
    static typename CreateImpl<T, TCallStack, TEntries>::ResultOf execute
    (
        TEntries& p_entries,
        TPool& p_pool,
        typename boost::disable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::disable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
    )
    {
        return CreateImpl<T, TCallStack, TEntries>::execute(p_entries, p_pool);
    }

    template<typename T, typename TCallStack, typename TEntries>
    static typename CreateImpl<T, TCallStack, TEntries>::ResultOf execute
    (
        TEntries& p_entries,
        TPool& p_pool,
        typename boost::disable_if< BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::enable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
    )
    {
        return CreateImpl<T, TCallStack, TEntries>::execute(p_entries, p_pool);
    }

    #define QDEPS_CREATE_IMPL_ARG(z, n, text) BOOST_PP_COMMA_IF(n)                                                      \
         Convert<typename boost::mpl::at_c<TCtor, n>::type>::execute(                                                   \
            execute<typename MakePlain<typename boost::mpl::at_c<TCtor, n>::type>::type, TCallStack>(p_e, p_pool)       \
         )

    #define QDEPS_CREATE_IMPL(z, n, text)                                                                               \
        template                                                                                                        \
        <                                                                                                               \
            typename T,                                                                                                 \
            typename TCtor,                                                                                             \
            typename TCallStack,                                                                                        \
            typename TEntries,                                                                                          \
            typename TE                                                                                                 \
        >                                                                                                               \
        static typename TEntries::template ResultType<TPool>::type execute                                              \
        (                                                                                                               \
            TE& BOOST_PP_EXPR_IF(n, p_e),                                                                               \
            TEntries& p_entries,                                                                                        \
            TPool& p_pool,                                                                                              \
            typename boost::enable_if_c<boost::mpl::size<TCtor>::value == n>::type* = 0                                 \
        )                                                                                                               \
        {                                                                                                               \
            return p_entries.create(                                                                                    \
                p_pool BOOST_PP_COMMA_IF(n)                                                                             \
                BOOST_PP_REPEAT(n, QDEPS_CREATE_IMPL_ARG, ~)                                                            \
            );                                                                                                          \
        }

    BOOST_PP_REPEAT(QDEPS_FUNCTION_ARITY_LIMIT_SIZE, QDEPS_CREATE_IMPL, ~)

    #undef QDEPS_CREATE_IMPL_ARG
    #undef QDEPS_CREATE_IMPL
};

} // namespace Detail

//TODO try inheritance
template
<
    typename TDeps,
    typename TPool
>
struct Create
{
    typedef typename Detail::Create<TDeps, TPool> type;
};

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif

