//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_EXTERNALS_HPP
#define BOOST_DI_CONCEPTS_EXTERNALS_HPP

#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace concepts {

namespace detail { class externals { }; } // namespace detail

template<BOOST_DI_ARGS_TYPES_MPL(T)>
struct externals
    : detail::externals, mpl::vector<BOOST_DI_ARGS_MPL(T)>
{ };

} // namespace concepts
} // namespace front
} // namespace di

#endif

