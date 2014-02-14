//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_HPP
#define BOOST_DI_HPP

#if !defined(BOOST_DI_PREPROCESSED)

    // inject
    #include "boost/di/inject.hpp"

    // concepts
    #include "boost/di/concepts.hpp"

    // scopes
    #include "boost/di/scopes/deduce.hpp"
    #include "boost/di/scopes/external.hpp"
    #include "boost/di/scopes/session.hpp"
    #include "boost/di/scopes/shared.hpp"
    #include "boost/di/scopes/unique.hpp"

    // injector
    #include "boost/di/injector.hpp"
    #include "boost/di/make_injector.hpp"

    // utilities
    #include "boost/di/named.hpp"
    #include "boost/di/provider.hpp"

    // policies
    #include "boost/di/policies/arguments_permission.hpp"
    #include "boost/di/policies/binding_correctness.hpp"
    #include "boost/di/policies/circular_dependencies.hpp"
    #include "boost/di/policies/creation_ownership.hpp"

    #include "boost/di/policies/scopes_permission.hpp"

#else

    // inject
    #include "boost/di_preprocessed/inject.hpp"

    // concepts
    #include "boost/di_preprocessed/concepts.hpp"

    // scopes
    #include "boost/di_preprocessed/scopes/deduce.hpp"
    #include "boost/di_preprocessed/scopes/external.hpp"
    #include "boost/di_preprocessed/scopes/session.hpp"
    #include "boost/di_preprocessed/scopes/shared.hpp"
    #include "boost/di_preprocessed/scopes/unique.hpp"

    // injector
    #include "boost/di_preprocessed/injector.hpp"
    #include "boost/di_preprocessed/make_injector.hpp"

    // utilities
    #include "boost/di_preprocessed/named.hpp"
    #include "boost/di_preprocessed/provider.hpp"

    // policies
    #include "boost/di_preprocessed/policies/arguments_permission.hpp"
    #include "boost/di_preprocessed/policies/binding_correctness.hpp"
    #include "boost/di_preprocessed/policies/circular_dependencies.hpp"
    #include "boost/di_preprocessed/policies/creation_ownership.hpp"
    #include "boost/di_preprocessed/policies/scopes_permission.hpp"

#endif

#endif

