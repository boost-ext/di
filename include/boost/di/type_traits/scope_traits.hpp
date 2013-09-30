//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>

#include "boost/di/type_traits/make_plain.hpp" // has_named_type
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/config.hpp"

namespace boost {
namespace di {
namespace type_traits {

template<typename T, typename = void>
struct scope_traits
{
    typedef scopes::per_request<> type;
};

template<typename T>
struct scope_traits<T&>
{
    typedef scopes::external<> type;
};

template<typename T>
struct scope_traits<const T&>
{
    typedef scopes::per_request<> type;
};

template<typename T>
struct scope_traits<T*>
{
    typedef scopes::per_request<> type;
};

template<typename T>
struct scope_traits<const T*>
{
    typedef scopes::per_request<> type;
};

template<typename T>
struct scope_traits<std::auto_ptr<T> >
{
    typedef scopes::per_request<> type;
};

template<typename T>
struct scope_traits<shared_ptr<T> >
{
    typedef scopes::singleton<> type;
};

template<typename T>
struct scope_traits<weak_ptr<T> >
{
    typedef scopes::singleton<> type;
};

#if !defined(BOOST_NO_CXX11_SMART_PTR)

template<typename T>
struct scope_traits<std::shared_ptr<T> >
{
    typedef scopes::singleton<> type;
};

template<typename T>
struct scope_traits<std::weak_ptr<T> >
{
    typedef scopes::singleton<> type;
};

template<typename T>
struct scope_traits<std::unique_ptr<T> >
{
    typedef scopes::per_request<> type;
};

#endif

#if !defined(BOOST_NO_RVALUE_REFERENCES)

template<typename T>
struct scope_traits<T&&>
{
    typedef scopes::per_request<> type;
};

template<typename T>
struct scope_traits<const T&&>
{
    typedef scopes::per_request<> type;
};

#endif

template<typename T>
struct scope_traits<T, typename enable_if<has_named_type<T> >::type>
{
    typedef typename scope_traits<typename T::named_type>::type type;
};

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

