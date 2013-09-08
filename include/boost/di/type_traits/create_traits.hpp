//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_TYPE_TRAITS_CREATE_TRAITS_HPP
    #define BOOST_DI_TYPE_TRAITS_CREATE_TRAITS_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/cat.hpp>
    #include <boost/utility/enable_if.hpp>

    #include "boost/di/type_traits/ctor_traits.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (               \
        BOOST_DI_ITERATION_PARAMS(                      \
            1                                           \
          , BOOST_DI_LIMIT_SIZE                         \
          , "boost/di/type_traits/create_traits.hpp"    \
        )                                               \
    )

    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename TExpected, typename TGiven>
    typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<TGiven>, TExpected*>::type
    create_traits() {
        return TGiven().BOOST_DI_CREATE();
    }

    template<typename TExpected, typename TGiven>
    typename disable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<TGiven>, TExpected*>::type
    create_traits() {
        return new TGiven();
    }

    #include BOOST_PP_ITERATE()

    } // namespace type_traits
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename TExpected, typename TGiven, BOOST_DI_TYPES(Args)>
    typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<TGiven>, TExpected*>::type
    create_traits(BOOST_DI_ARGS(Args, args)) {
        return TGiven().BOOST_DI_CREATE(BOOST_DI_ARGS_FORWARD(args));
    }

    template<typename TExpected, typename TGiven, BOOST_DI_TYPES(Args)>
    typename disable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<TGiven>, TExpected*>::type
    create_traits(BOOST_DI_ARGS(Args, args)) {
        return new TGiven(BOOST_DI_ARGS_FORWARD(args));
    }

#endif

