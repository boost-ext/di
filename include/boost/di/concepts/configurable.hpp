//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CONFIGURABLE_HPP
#define BOOST_DI_CONCEPTS_CONFIGURABLE_HPP

#include <type_traits>

namespace boost { namespace di { namespace concepts {

BOOST_DI_HAS_METHOD(policies, policies);
BOOST_DI_HAS_METHOD(provider, provider);

template<class T>
using configurable =
    std::integral_constant<bool, has_policies<T>{} && has_provider<T>{}>;

}}} // boost::di::concepts

#endif

