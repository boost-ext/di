//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_REBIND_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_REBIND_TRAITS_HPP

#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace type_traits {

template<class T, class U>
struct t_traits {
    using type = U;
    using type_ = U;
};

template<class T, class U>
struct t_traits<T, named<U>> {
    using type = di::detail::named_type<U, T>;
    using type_ = T;
};

template<class T, class U>
struct t_traits<std::shared_ptr<T>, U> {
    using type = std::shared_ptr<U>;
    using type_ = std::shared_ptr<U>;
};

template<class T, class U>
struct t_traits<std::shared_ptr<T>, named<U>> {
    using type = di::detail::named_type<U, std::shared_ptr<T>>;
    using type_ = std::shared_ptr<T>;
};

template<class T, class D, class U>
struct t_traits<std::unique_ptr<T, D>, U> {
    using type = std::unique_ptr<U, D>;
    using type_ = std::unique_ptr<U, D>;
};

template<class T, class D, class U>
struct t_traits<std::unique_ptr<T, D>, named<U>> {
    using type = di::detail::named_type<U, std::unique_ptr<T, D>>;
    using type_ = std::unique_ptr<T, D>;
};

template<class T, class U>
using t_traits_t = typename t_traits<T, U>::type;

template<class T, class U>
using t_traits_t_ = typename t_traits<T, U>::type_;

}}}} // boost::di::v1::type_traits

#endif


