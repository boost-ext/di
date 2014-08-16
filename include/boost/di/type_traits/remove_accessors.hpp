//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_REMOVE_ACCESSORS_HPP
#define BOOST_DI_TYPE_TRAITS_REMOVE_ACCESSORS_HPP

#include <type_traits>

namespace boost {
namespace di {
namespace type_traits {

template<typename T>
using remove_accessors = std::remove_cv<
   typename std::remove_pointer<
       typename std::remove_reference<T>::type
   >::type
>;

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

