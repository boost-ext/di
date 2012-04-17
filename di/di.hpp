//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_DI_HPP
#define DI_DI_HPP

#include "di/version.hpp"
#include "di/config.hpp"

// front ends
#include "di/front/ctor.hpp"
#include "di/front/base/module.hpp"
#include "di/front/fusion/module.hpp"

// utility
#include "di/utility/injector.hpp"
#include "di/utility/named.hpp"
#include "di/utility/provider.hpp"

// policies
#include "di/back/policies/check_for_binding_correctness.hpp"
#include "di/back/policies/check_for_circular_dependencies.hpp"
#include "di/back/policies/check_for_creation_ownership.hpp"
#include "di/back/policies/check_for_references_without_ownership.hpp"

#endif

