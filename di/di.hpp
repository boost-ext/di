//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_DI_HPP
#define DI_DI_HPP

#include "di/Version.hpp"
#include "di/Config.hpp"

// Front ends
#include "di/Front/Ctor.hpp"
#include "di/Front/Base/Module.hpp"
#include "di/Front/Fusion/Module.hpp"

// Utility
#include "di/Utility/Injector.hpp"
#include "di/Utility/Named.hpp"
#include "di/Utility/Provider.hpp"

// Policies
#include "di/Back/Policies/CheckForBindingCorrectness.hpp"
#include "di/Back/Policies/CheckForCircularDependencies.hpp"
#include "di/Back/Policies/CheckForCreationOwnership.hpp"
#include "di/Back/Policies/CheckForReferencesWithoutOwnership.hpp"

#endif

