//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_HPP
#define BOOST_DI_HPP

// ctor
#include "boost/di/ctor.hpp"

// concepts
#include "boost/di/concepts.hpp"

// scopes
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/singleton.hpp"

// injector
#include "boost/di/injector.hpp"
#include "boost/di/make_injector.hpp"

// utilities
#include "boost/di/named.hpp"
#include "boost/di/provider.hpp"

// policies
#include "boost/di/policies/arguments_allowance.hpp"
#include "boost/di/policies/binding_correctness.hpp"
#include "boost/di/policies/circular_dependencies.hpp"
#include "boost/di/policies/creation_ownership.hpp"
#include "boost/di/policies/undefined_behaviors.hpp"

#endif

