//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TEST_CONFIG_HPP
#define BOOST_DI_TEST_CONFIG_HPP

#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_SMART_PTR) &&    \
    __clang_major__ >= 3 &&                 \
    __clang_minor__ >= 2 &&                 \
    __cplusplus >= 201100L
    #undef BOOST_NO_CXX11_SMART_PTR
#endif

#if defined(BOOST_GCC) || defined(BOOST_CLANG)
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations" // std::aux::auto_ptr
#endif

#if defined(BOOST_INTEL)
    #pragma warning(disable:1478) // class "std::aux::auto_ptr<...>" was declared deprecated
#endif

#if defined(BOOST_MSVC)
    #pragma warning(disable:4127) // conditional expression is constant
#endif

#if defined(BOOST_DI_CFG_TEST_CTOR)
    #if defined(BOOST_DI_CFG_INJECT_BRACKET)
        #define BOOST_DI_INJECT(type, args) type args
    #else
        #define BOOST_DI_INJECT(type, ...) type(__VA_ARGS__)
    #endif
#endif

#if defined(BOOST_MPL_LIMIT_VECTOR_SIZE)

    #if (BOOST_MPL_LIMIT_VECTOR_SIZE <= 50)
        #if !defined(BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS)
            #define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
        #endif
        #include <boost/mpl/vector.hpp>
    #else
        #include <boost/preprocessor/iteration/iterate.hpp>
        #include <boost/mpl/vector/vector50.hpp>

        namespace boost {
        namespace mpl {
            #define BOOST_PP_FILENAME_1 <boost/mpl/vector/aux_/numbered.hpp>
            #define BOOST_PP_ITERATION_LIMITS (51, BOOST_MPL_LIMIT_VECTOR_SIZE)
            #include BOOST_PP_ITERATE()
        } // namespace mpl
        } // namespace boost

        #define BOOST_MPL_PREPROCESSING_MODE
        #include <boost/mpl/vector.hpp>
        #undef BOOST_MPL_PREPROCESSING_MODE
    #endif

#else
    #include <boost/mpl/limits/vector.hpp> // default BOOST_MPL_LIMIT_VECTOR_SIZE=20
#endif

#endif

