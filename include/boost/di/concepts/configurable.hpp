//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CONFIGURABLE_HPP
#define BOOST_DI_CONCEPTS_CONFIGURABLE_HPP

#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { namespace concepts {

std::false_type configurable(...);

template<class T>
auto configurable(T&& t) -> aux::is_valid_expr<
    decltype(t.provider())
  , decltype(t.policies())
  , decltype(t.visitors())
>;

}}} // boost::di::concepts

#endif

