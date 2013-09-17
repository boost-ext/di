//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_REMOVE_ACCESSORS_HPP
#define BOOST_DI_TYPE_TRAITS_REMOVE_ACCESSORS_HPP

#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost {
namespace di {
namespace type_traits {

template<typename T>
struct remove_accessors
    : remove_cv<
        typename remove_pointer<
            typename remove_reference<T>::type
        >::type
      >
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

