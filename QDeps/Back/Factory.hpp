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
#include <boost/utility/enable_if.hpp>
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
#include "QDeps/Back/Aux/Inst.hpp"
#include "QDeps/Back/Aux/Impl.hpp"
#include "QDeps/Back/Utility.hpp"
#include "QDeps/Back/Shared.hpp"
#include "QDeps/Back/Creator.hpp"
#include "QDeps/Utility/Scopes/PerRequest.hpp"
#include "QDeps/Config.hpp"

namespace QDeps
{
namespace Back
{

template
<
    typename TDeps,
    typename TPool = boost::none_t const,
    typename TShared = Shared,
    template<typename, typename, typename = TDeps> class TCreator = Creator
>
class Factory
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(QDEPS_CTOR_UNIQUE_NAME)

    template<typename T> struct MakePlain
    {
        typedef typename RemoveAccessors<typename TShared::template Deref<typename RemoveAccessors<T>::type>::type>::type type;
    };

    template<typename T> struct MakeShared
    {
        typedef typename TShared::template Make<typename MakePlain<T>::type>::type type;
    };

    struct Entries : boost::mpl::inherit_linearly
        <
            TDeps,
            boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2>
        >::type
    { };

public:
    explicit Factory(TPool& p_pool = boost::none)
        : m_pool(p_pool)
    { }

    template<typename T> T create()
    {
        typedef boost::mpl::vector0<> EmptyCallStack;
        typedef typename MakePlain<T>::type PlainType;
        return TShared::template Convert<T>::execute(createImpl<PlainType, EmptyCallStack>());
    }

private:
    template<typename T, typename TCallStack> typename MakeShared<T>::type createImpl
    (
        typename boost::enable_if< BOOST_PP_CAT(has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::enable_if< boost::mpl::empty<typename TCreator<T, TCallStack>::type> >::type* = 0
    )
    {
        typedef typename boost::function_types::parameter_types<typename T::QDEPS_CTOR_UNIQUE_NAME>::type Ctor;
        typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
        return createImpl<T, Ctor, CallStack>(Aux::Inst<Utility::Scopes::PerRequest, T>());
    }

    template<typename T, typename TCallStack> typename MakeShared<T>::type createImpl
    (
        typename boost::enable_if< BOOST_PP_CAT(has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::disable_if< boost::mpl::empty<typename TCreator<T, TCallStack>::type> >::type* = 0
    )
    {
        typedef typename boost::function_types::parameter_types<typename T::QDEPS_CTOR_UNIQUE_NAME>::type Ctor;
        typedef typename TCreator<T, TCallStack>::type Creator;
        typedef typename boost::mpl::deref<typename boost::mpl::begin<Creator>::type>::type ToBeCreated;
        typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
        return createImpl<T, Ctor, CallStack>(static_cast<ToBeCreated&>(m_entries));
    }

    template<typename T, typename TCallStack> typename MakeShared<T>::type createImpl
    (
        typename boost::disable_if< BOOST_PP_CAT(has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::enable_if< boost::mpl::empty<typename TCreator<T, TCallStack>::type> >::type* = 0
    )
    {
        return Defaults<T, Specialized>::create();
    }

    template<typename T, typename TCallStack> typename MakeShared<T>::type createImpl
    (
        typename boost::disable_if< BOOST_PP_CAT(has_, QDEPS_CTOR_UNIQUE_NAME)<T> >::type* = 0,
        typename boost::disable_if< boost::mpl::empty<typename TCreator<T, TCallStack>::type> >::type* = 0
    )
    {
        typedef typename boost::mpl::vector0<> Ctor;
        typedef typename TCreator<T, TCallStack>::type Creator;
        typedef typename boost::mpl::deref<typename boost::mpl::begin<Creator>::type>::type ToBeCreated;
        typedef typename boost::mpl::push_back<TCallStack, T>::type CallStack;
        return createImpl<T, Ctor, CallStack>(static_cast<ToBeCreated&>(m_entries));
    }

    #define QDEPS_CREATE_IMPL_ARG(z, n, text) BOOST_PP_COMMA_IF(n)                                          \
         TShared::template Convert<typename boost::mpl::at_c<Ctor, n>::type>::execute(                      \
             createImpl<typename MakePlain<typename boost::mpl::at_c<Ctor, n>::type>::type, TCallStack>()   \
         )

    #define QDEPS_CREATE_IMPL(z, n, text)                                                                   \
        template<typename T, typename Ctor, typename TCallStack, typename TEntry>                           \
        typename MakeShared<T>::type createImpl                                                             \
        (                                                                                                   \
            TEntry p_entry,                                                                                 \
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

