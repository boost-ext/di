// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP

#include "boost/di/fwd.hpp"
#include "boost/di/scopes/unique.hpp"
#include "boost/di/scopes/singleton.hpp"

namespace boost { namespace di { inline namespace v1 { namespace type_traits {

template<class T>
struct scope_traits {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<T&> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const T&> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<T*> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<const T*> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<T&&> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<const T&&> {
    using type = scopes::unique;
};

template<class T, class TDeleter>
struct scope_traits<std::unique_ptr<T, TDeleter>> {
    using type = scopes::unique;
};

template<class T, class TDeleter>
struct scope_traits<const std::unique_ptr<T, TDeleter>&> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<std::shared_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const std::shared_ptr<T>&> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<boost::shared_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const boost::shared_ptr<T>&> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<std::weak_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const std::weak_ptr<T>&> {
    using type = scopes::singleton;
};

template<class T>
using scope_traits_t = typename scope_traits<T>::type;

}}}} // boost::di::v1::type_traits

#endif

