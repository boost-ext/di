//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_HPP
#define BOOST_DI_HPP

// core
#include "di/config.hpp"
#include "di/ctor.hpp"
#include "di/injector.hpp"

// utility
#include "di/named.hpp"
#include "di/provider.hpp"

// module
#include "di/module/base.hpp"
#include "di/module/fusion.hpp"

// policy
#include "di/policy/check_for_binding_correctness.hpp"
#include "di/policy/check_for_circular_dependencies.hpp"
#include "di/policy/check_for_creation_ownership.hpp"
#include "di/policy/check_for_references_without_ownership.hpp"

#endif

