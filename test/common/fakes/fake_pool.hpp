//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_POOL_HPP
#define BOOST_DI_FAKE_POOL_HPP

#include "boost/di/aux_/utility.hpp"

template <class... Ts>
using fake_pool = aux::type_list<Ts...>;

#endif
