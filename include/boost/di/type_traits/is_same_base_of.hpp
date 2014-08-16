//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_IS_SAME_BASE_OF_HPP
#define BOOST_DI_TYPE_TRAITS_IS_SAME_BASE_OF_HPP

#include "boost/di/aux_/mpl.hpp"

namespace boost {
namespace di {
namespace type_traits {

template<typename T, typename U>
using is_same_base_of = bool_<
    std::is_base_of<U, T>::value || std::is_same<U, T>::value
>;

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

