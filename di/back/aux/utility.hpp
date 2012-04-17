//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_BACK_AUX_utILITY_HPP
#define DI_BACK_AUX_utILITY_HPP

#include <boost/shared_ptr.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include "di/config.hpp"

namespace di
{
namespace back
{
namespace aux
{

template<typename T> struct get_bind
{
    typedef typename T::bind type;
};

template<typename T> struct get_pool
{
    typedef typename T::pool type;
};

template<typename T> struct get_context
{
    typedef typename T::context type;
};

template<typename T> struct get_context_size
{
    static const std::size_t value = boost::mpl::size<typename get_context<T>::type>::value;
};

template<typename T> struct get_dependencies
{
    typedef typename T::Dependencies type;
};

template<typename T> struct get_externals
{
    typedef typename T::Externals type;
};

template<typename T> struct get_derived
{
    typedef typename T::derived type;
};

template<typename T> struct get_seq
{
    typedef typename T::seq type;
};

template<typename TDependency, int N, typename TResult = void> struct enable_if_ctor_size
    : boost::enable_if_c<boost::mpl::size<typename TDependency::ctor>::value == N, TResult>
{ };

template<typename TDependency, int N> struct at_ctor
    : boost::mpl::at_c<typename TDependency::ctor, N>
{ };

template<typename TCallStack, typename TDependency> struct update_call_stack
    : boost::mpl::push_back<TCallStack, typename TDependency::given>
{ };

template<typename TElement> class make_plain
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

    template<typename T> struct remove_accessors
    {
        typedef typename boost::remove_cv<typename boost::remove_pointer<typename boost::remove_reference<T>::type>::type>::type type;
    };

    template<typename T, typename Enable = void> struct deref_element_type
    {
        typedef T type;
    };

    template<typename T> struct deref_element_type<T, typename boost::enable_if< has_element_type<T> >::type>
    {
        typedef typename T::element_type type;
    };

public:
    typedef typename deref_element_type<typename remove_accessors<typename deref_element_type<typename remove_accessors<TElement>::type>::type>::type>::type type;
};

} // namespace aux
} // namespace back
} // namespace di

#endif

