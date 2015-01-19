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

template<class Concept, class Enable=void>
struct models
: std::false_type
{};
template<class T>
struct always_void {
    typedef void type;
};

template<class Concept, class... Ts>
struct models<Concept(Ts...), typename always_void<
    decltype(std::declval<Concept>().requires_(std::declval<Ts>()...))
>::type>
: std::true_type
{};

struct configurable_ {
    template<class T>
    auto requires_(T&& x) -> aux::void_t<
        decltype(x.policies())
      , decltype(x.provider())
    >;
};

template<class T>
struct configurable : models<configurable_(T)>
{ };

}}} // boost::di::concepts

#endif

