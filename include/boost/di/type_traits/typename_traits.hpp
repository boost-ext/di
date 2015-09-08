//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_TYPENAME_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_TYPENAME_TRAITS_HPP

#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace type_traits {

template<class T, class>
struct typename_traits {
    using type = T;
};

template<class T>
struct typename_traits<_, T> {
    using type = T;
};

template<class T>
struct typename_traits<_&, T> {
    using type = T&;
};

template<class T>
struct typename_traits<const _&, T> {
    using type = const T&;
};

template<class T>
struct typename_traits<_&&, T> {
    using type = T&&;
};

template<class T>
struct typename_traits<_*, T> {
    using type = T*;
};

template<class T>
struct typename_traits<const _*, T> {
    using type = const T*;
};

template<class T, template<class...> class TDeleter>
struct typename_traits<std::unique_ptr<_, TDeleter<_>>, T> {
    using type = std::unique_ptr<T, TDeleter<T>>;
};

template<class T, template<class...> class TDeleter>
struct typename_traits<const std::unique_ptr<_, TDeleter<_>>&, T> {
    using type = const std::unique_ptr<T, TDeleter<T>>&;
};

template<class T>
struct typename_traits<std::shared_ptr<_>, T> {
    using type = std::shared_ptr<T>;
};

template<class T>
struct typename_traits<const std::shared_ptr<_>&, T> {
    using type = std::shared_ptr<T>;
};

template<class T>
struct typename_traits<boost::shared_ptr<_>, T> {
    using type = boost::shared_ptr<T>;
};

template<class T>
struct typename_traits<const boost::shared_ptr<_>&, T> {
    using type = boost::shared_ptr<T>;
};

template<class T>
struct typename_traits<std::weak_ptr<_>, T> {
    using type = std::weak_ptr<T>;
};

template<class T>
struct typename_traits<const std::weak_ptr<_>&, T> {
    using type = std::weak_ptr<T>;
};

template<class T, class U>
using typename_traits_t = typename typename_traits<T, U>::type;

}}}} // boost::di::v1::type_traits

#endif

