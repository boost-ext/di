//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_BACK_AUX_UTILITY_HPP
#define DI_BACK_AUX_UTILITY_HPP

#include <boost/shared_ptr.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include "di/Config.hpp"

namespace di
{
namespace Back
{
namespace Aux
{

template<typename T> struct GetBind
{
    typedef typename T::Bind type;
};

template<typename T> struct GetPool
{
    typedef typename T::Pool type;
};

template<typename T> struct GetContext
{
    typedef typename T::Context type;
};

template<typename T> struct GetContextSize
{
    static const std::size_t value = boost::mpl::size<typename GetContext<T>::type>::value;
};

template<typename T> struct GetDependencies
{
    typedef typename T::Dependencies type;
};

template<typename T> struct GetExternals
{
    typedef typename T::Externals type;
};

template<typename T> struct GetDerived
{
    typedef typename T::Derived type;
};

template<typename T> struct GetSeq
{
    typedef typename T::Seq type;
};

template<typename TDependency, int N, typename TResult = void> struct EnableIfCtorSize
    : boost::enable_if_c<boost::mpl::size<typename TDependency::Ctor>::value == N, TResult>
{ };

template<typename TDependency, int N> struct AtCtor
    : boost::mpl::at_c<typename TDependency::Ctor, N>
{ };

template<typename TCallStack, typename TDependency> struct UpdateCallStack
    : boost::mpl::push_back<TCallStack, typename TDependency::Given>
{ };

template<typename TElement> class MakePlain
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

    template<typename T> struct RemoveAccessors
    {
        typedef typename boost::remove_cv<typename boost::remove_pointer<typename boost::remove_reference<T>::type>::type>::type type;
    };

    template<typename T, typename Enable = void> struct DerefElementType
    {
        typedef T type;
    };

    template<typename T> struct DerefElementType<T, typename boost::enable_if< has_element_type<T> >::type>
    {
        typedef typename T::element_type type;
    };

public:
    typedef typename DerefElementType<typename RemoveAccessors<typename DerefElementType<typename RemoveAccessors<TElement>::type>::type>::type>::type type;
};

} // namespace Aux
} // namespace Back
} // namespace di

#endif

