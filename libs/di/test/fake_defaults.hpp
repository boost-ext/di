//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DEFAULTS_HPP
#define BOOST_DI_FAKE_DEFAULTS_HPP

#include "boost/di/defaults.hpp"

#include "fake_allocator.hpp"

namespace boost {
namespace di {

template<>
struct defaults<specialized>
{
    typedef policy<> policies;

    template<typename T>
    struct allocator
        : fake_allocator<T>
    { };
};

} // namespace di
} // namespace boost

#endif

