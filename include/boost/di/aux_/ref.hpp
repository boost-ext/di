//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_REF_HPP
#define BOOST_DI_AUX_REF_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/mpl.hpp"
#include <functional>

#if !defined(BOOST_DI_CFG_NO_BOOST)
    #include <boost/ref.hpp>
#endif

namespace boost {
namespace di {
namespace aux {

template<typename>
struct is_reference_wrapper
    : std::false_type
{ };

template<typename T>
struct is_reference_wrapper<::std::reference_wrapper<T>>
    : std::true_type
{ };

#if !defined(BOOST_DI_CFG_NO_BOOST)
    template<typename T>
    struct is_reference_wrapper<::boost::reference_wrapper<T>>
        : std::true_type
    { };
#endif

template<typename T>
struct unwrap_reference {
    using type = T;
};

template<typename T>
struct unwrap_reference<::std::reference_wrapper<T>> {
    using type = T;
};

#if !defined(BOOST_DI_CFG_NO_BOOST)
    template<typename T>
    struct unwrap_reference<::boost::reference_wrapper<T>> {
        using type = T;
    };
#endif

} // namespace aux
} // namespace boost
} // namespace di

#endif

