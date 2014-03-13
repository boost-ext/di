//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_HPP
#define BOOST_DI_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#include "boost/di/aux_/config.hpp"

#if !defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS) &&       \
    (                                                       \
        BOOST_DI_CFG_CTOR_LIMIT_SIZE == 10                  \
    )                                                       \
    &&                                                      \
    (                                                       \
        BOOST_MPL_LIMIT_VECTOR_SIZE >= 20 &&                \
        BOOST_MPL_LIMIT_VECTOR_SIZE <= 50 &&                \
        BOOST_MPL_LIMIT_VECTOR_SIZE % 10 == 0               \
    )

    #include BOOST_PP_STRINGIZE(                            \
        BOOST_PP_CAT(boost/di/preprocessed/di_,             \
        BOOST_PP_CAT(BOOST_DI_CFG_CTOR_LIMIT_SIZE,          \
        BOOST_PP_CAT(_,                                     \
        BOOST_PP_CAT(BOOST_MPL_LIMIT_VECTOR_SIZE, _c.hpp    \
    )))))

#else

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

#endif

#endif

