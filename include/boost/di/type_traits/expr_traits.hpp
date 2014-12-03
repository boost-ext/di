//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_EXPR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_EXPR_TRAITS_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"

namespace boost { namespace di { namespace type_traits {

struct ptr { };
struct value { };

template<class T>
struct expr_traits {
    using type = value;
};

template<class T>
struct expr_traits<T&> {
    using type = value;
};

template<class T>
struct expr_traits<const T&> {
    using type = value;
};

template<class T>
struct expr_traits<T*> {
    using type = ptr;
};

template<class T>
struct expr_traits<const T*> {
    using type = ptr;
};

template<class T>
struct expr_traits<aux::shared_ptr<T>> {
    using type = ptr;
};

template<class T>
struct expr_traits<const aux::shared_ptr<T>&> {
    using type = ptr;
};

template<class T>
struct expr_traits<aux_::shared_ptr<T>> {
    using type = ptr;
};

template<class T>
struct expr_traits<const aux_::shared_ptr<T>&> {
    using type = ptr;
};

template<class T>
struct expr_traits<aux::weak_ptr<T>> {
    using type = ptr;
};

template<class T>
struct expr_traits<const aux::weak_ptr<T>&> {
    using type = ptr;
};

template<class T>
struct expr_traits<aux::unique_ptr<T>> {
    using type = ptr;
};

template<class T>
struct expr_traits<const aux::unique_ptr<T>&> {
    using type = ptr;
};

template<class T>
struct expr_traits<T&&> {
    using type = value;
};

template<class T>
struct expr_traits<const T&&> {
    using type = value;
};

template<class T, class TName>
struct expr_traits<named<T, TName>> {
    using type = typename expr_traits<T>::type;
};

template<class T, class TName>
struct expr_traits<const named<T, TName>&> {
    using type = typename expr_traits<T>::type;
};

template<class T>
using expr_traits_t = typename expr_traits<T>::type;

}}} // namespace boost::di::type_traits

#endif

