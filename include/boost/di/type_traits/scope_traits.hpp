//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/scopes/unique.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/named.hpp"

namespace boost { namespace di { namespace type_traits {

template<class T>
struct scope_traits {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<T&> {
    using type = scopes::external;
};

template<class T>
struct scope_traits<const T&> {
    using type = scopes::unique;
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
struct scope_traits<aux::shared_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const aux::shared_ptr<T>&> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<aux_::shared_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const aux_::shared_ptr<T>&> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<aux::weak_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const aux::weak_ptr<T>&> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<aux::unique_ptr<T>> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<const aux::unique_ptr<T>&> {
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

template<class T, class TName>
struct scope_traits<named<T, TName>> {
    using type = typename scope_traits<T>::type;
};

template<class T, class TName>
struct scope_traits<const named<T, TName>&> {
    using type = typename scope_traits<T>::type;
};

template<class T>
using scope_traits_t = typename scope_traits<T>::type;

}}} // namespace boost::di::type_traits

#endif

