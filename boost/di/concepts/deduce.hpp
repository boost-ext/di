//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_DEDUCE_HPP
#define BOOST_DI_CONCEPTS_DEDUCE_HPP

#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace concepts {

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct deduce
{ };

} // namespace concepts
} // namespace front
} // namespace di

#endif

