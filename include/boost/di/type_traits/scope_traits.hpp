//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP

#include <memory>
#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>

#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/external.hpp"

namespace boost {
namespace di {
namespace type_traits {

template<typename T>
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
struct scope_traits<boost::shared_ptr<T> >
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

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

