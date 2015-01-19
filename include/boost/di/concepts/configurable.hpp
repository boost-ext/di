//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CONFIGURABLE_HPP
#define BOOST_DI_CONCEPTS_CONFIGURABLE_HPP

#include <type_traits>
#include "boost/di/aux_/utility.hpp"

namespace boost { namespace di { namespace concepts {

template<class, class = void>
struct configurable : std::false_type { };

template<class T>
struct configurable<T, aux::void_t<decltype(std::declval<T>().policies()), decltype(std::declval<T>().provider())>>
    : std::true_type
{ };

}}} // boost::di::concepts

#endif

