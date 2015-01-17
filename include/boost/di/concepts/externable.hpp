//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_EXTERNABLE_HPP
#define BOOST_DI_CONCEPTS_EXTERNABLE_HPP

#include <type_traits>

namespace boost { namespace di { namespace concepts {

template<class TExpected, class TGiven, class TScope>
using externable = std::integral_constant<bool, std::is_same<TExpected, TGiven>{} && std::is_same<TScope, scopes::deduce>{}>;

}}} // boost::di::concepts

#endif

