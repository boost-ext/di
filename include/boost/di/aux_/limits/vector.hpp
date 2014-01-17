//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_LIMIT_VECTOR_HPP
#define BOOST_DI_AUX_LIMIT_VECTOR_HPP

#include <boost/preprocessor/iteration/iterate.hpp>

#if defined(BOOST_MPL_LIMIT_VECTOR_SIZE)

    #if (BOOST_MPL_LIMIT_VECTOR_SIZE <= 50)
        #if !defined(BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS)
            #define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
        #endif
        #include <boost/mpl/vector.hpp>
    #else
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

