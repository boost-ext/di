//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_QDEPS_HPP
#define QDEPS_QDEPS_HPP

#include "QDeps/Version.hpp"
#include "QDeps/Config.hpp"

//Front ends
#include "QDeps/Front/Ctor.hpp"
#include "QDeps/Front/Base/Module.hpp"
#include "QDeps/Front/Fusion/Module.hpp"

//Utility
#include "QDeps/Utility/Injector.hpp"
#include "QDeps/Utility/Named.hpp"
#include "QDeps/Utility/Provider.hpp"

//Policies
#include "QDeps/Back/Policies/CheckForBindingCorrectness.hpp"
#include "QDeps/Back/Policies/CheckForCircularDependencies.hpp"
#include "QDeps/Back/Policies/CheckForCreationOwnership.hpp"
#include "QDeps/Back/Policies/CheckForReferencesWithoutOwnership.hpp"

#endif

