//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CONFIG_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CONFIG_TRAITS_HPP

namespace boost { namespace di { namespace type_traits {

template<class TConfig, class>
struct config_traits {
    using type = TConfig;
};

template<template<class> class TConfig, class T, class TInjector>
struct config_traits<TConfig<T>, TInjector> {
    using type = TConfig<TInjector>;
};

}}} // boost::di::type_traits

#endif

