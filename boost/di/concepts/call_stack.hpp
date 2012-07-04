//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CALL_STACK_HPP
#define BOOST_DI_CONCEPTS_CALL_STACK_HPP

#include <boost/mpl/vector.hpp>

#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace concepts {

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct call_stack
    : mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

} // namespace concepts
} // namespace front
} // namespace di

#endif

