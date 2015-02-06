//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_CONFIG_HPP
#define BOOST_DI_FAKE_CONFIG_HPP

#include "boost/di/providers/heap.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di {

template<class = void>
struct fake_config {
    auto policies() const {
        return make_policies();
    }

    auto provider() const {
        return providers::heap{};
    }
};

}} // boost::di

#endif

