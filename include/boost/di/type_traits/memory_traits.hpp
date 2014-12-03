//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_MEMORY_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_MEMORY_TRAITS_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"

namespace boost { namespace di { namespace type_traits {

struct heap { };
struct stack { };

template<class T>
struct memory_traits {
    using type = stack;
};

template<class T>
struct memory_traits<T&> {
    using type = stack;
};

template<class T>
struct memory_traits<const T&> {
    using type = stack;
};

template<class T>
struct memory_traits<T*> {
    using type = heap;
};

template<class T>
struct memory_traits<const T*> {
    using type = heap;
};

template<class T>
struct memory_traits<aux::shared_ptr<T>> {
    using type = heap;
};

template<class T>
struct memory_traits<const aux::shared_ptr<T>&> {
    using type = heap;
};

template<class T>
struct memory_traits<aux_::shared_ptr<T>> {
    using type = heap;
};

template<class T>
struct memory_traits<const aux_::shared_ptr<T>&> {
    using type = heap;
};

template<class T>
struct memory_traits<aux::weak_ptr<T>> {
    using type = heap;
};

template<class T>
struct memory_traits<const aux::weak_ptr<T>&> {
    using type = heap;
};

template<class T>
struct memory_traits<aux::unique_ptr<T>> {
    using type = heap;
};

template<class T>
struct memory_traits<const aux::unique_ptr<T>&> {
    using type = heap;
};

template<class T>
struct memory_traits<T&&> {
    using type = stack;
};

template<class T>
struct memory_traits<const T&&> {
    using type = stack;
};

template<class T, class TName>
struct memory_traits<named<T, TName>> {
    using type = typename memory_traits<T>::type;
};

template<class T, class TName>
struct memory_traits<const named<T, TName>&> {
    using type = typename memory_traits<T>::type;
};

template<class T>
using memory_traits_t = typename memory_traits<T>::type;

}}} // namespace boost::di::type_traits

#endif

