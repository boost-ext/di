//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_FACTORY_HPP
#define QDEPS_BACK_FACTORY_HPP

#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/none.hpp>
#include <boost/make_shared.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/has_xxx.hpp>
#include "QPool/Pool.hpp"
#include "QDeps/Back/Utility.hpp"
#include "QDeps/Back/Convert.hpp"
#include "QDeps/Back/Binding.hpp"
#include "QDeps/Back/Dependency.hpp"
#include "QDeps/Back/Scope/PerRequest.hpp"

namespace QDeps
{
namespace Back
{

template
<
    typename TDeps,
    typename TPool = const QPool::Pool<>,
    template<typename> class TConvert = Convert,
    template<typename, typename, typename = TDeps> class TBinding = Binding
>
class Factory
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(QDEPS_CTOR_UNIQUE_NAME)

    struct Entries : boost::mpl::inherit_linearly
        <
            TDeps,
            boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2>
        >::type
    { };

    template<typename T> struct Ctor
    {
        typedef BOOST_TYPEOF_TPL(T::QDEPS_CTOR_UNIQUE_NAME::ctor) type;
    };

public:
    explicit Factory(TPool& p_pool = TPool())
        : m_pool(p_pool)
    { }

    template<typename T> T create()
    {
        typedef boost::mpl::vector0<> EmptyCallStack;
        typedef typename MakePlain<T>::type PlainType;
        return Convert<T>::execute(createImpl<PlainType, EmptyCallStack>());
    }

private:
    template<typename T, typename TCallStack> typename MakeShared<T>::type createImpl
    (
        typename boost::enable_if< BOOST_PP_CAT(has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::enable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
    )
    {
        typedef typename boost::function_types::parameter_types<typename Ctor<T>::type>::type Ctor;
        typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
        Dependency<Scope::PerRequest, T> l_onDemandInst;
        return createImpl<T, Ctor, CallStack>(l_onDemandInst);
    }

    template<typename T, typename TCallStack> typename MakeShared<T>::type createImpl
    (
        typename boost::enable_if< BOOST_PP_CAT(has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::disable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
    )
    {
        typedef typename boost::function_types::parameter_types<typename Ctor<T>::type>::type Ctor;
        typedef typename TBinding<T, TCallStack>::type Binding;
        typedef typename boost::mpl::deref<typename boost::mpl::begin<Binding>::type>::type ToBeCreated;
        typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
        return createImpl<T, Ctor, CallStack>(static_cast<ToBeCreated&>(m_entries));
    }

    template<typename T, typename TCallStack> typename MakeShared<T>::type createImpl
    (
        typename boost::disable_if< BOOST_PP_CAT(has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::enable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
    )
    {
        return boost::make_shared<T>();
    }

    template<typename T, typename TCallStack> typename MakeShared<T>::type createImpl
    (
        typename boost::disable_if< BOOST_PP_CAT(has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::disable_if< boost::mpl::empty<typename TBinding<T, TCallStack>::type> >::type* = 0
    )
    {
        typedef typename boost::mpl::vector0<> Ctor;
        typedef typename TBinding<T, TCallStack>::type Binding;
        typedef typename boost::mpl::deref<typename boost::mpl::begin<Binding>::type>::type ToBeCreated;
        typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
        return createImpl<T, Ctor, CallStack>(static_cast<ToBeCreated&>(m_entries));
    }

    #define QDEPS_CREATE_IMPL_ARG(z, n, text) BOOST_PP_COMMA_IF(n)                                          \
         Convert<typename boost::mpl::at_c<Ctor, n>::type>::execute(                                        \
             createImpl<typename MakePlain<typename boost::mpl::at_c<Ctor, n>::type>::type, TCallStack>()   \
         )

    #define QDEPS_CREATE_IMPL(z, n, text)                                                                   \
        template<typename T, typename Ctor, typename TCallStack, typename TEntry>                           \
        typename MakeShared<T>::type createImpl                                                             \
        (                                                                                                   \
            TEntry& p_entry,                                                                                \
            typename boost::enable_if_c<boost::mpl::size<Ctor>::value == n>::type* = 0                      \
        )                                                                                                   \
        {                                                                                                   \
            return p_entry.create(                                                                          \
                m_pool BOOST_PP_COMMA_IF(n)                                                                 \
                BOOST_PP_REPEAT(n, QDEPS_CREATE_IMPL_ARG, ~)                                                \
            );                                                                                              \
        }

    BOOST_PP_REPEAT(QDEPS_FUNC_ARITY, QDEPS_CREATE_IMPL, ~)

    #undef QDEPS_CREATE_IMPL_ARG
    #undef QDEPS_CREATE_IMPL

private:
    TPool& m_pool;
    Entries m_entries;
};

} // namespace Back
} // namespace QDeps

#endif

