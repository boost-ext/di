//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_MAKE_INJECTOR_HPP
    #define BOOST_DI_MAKE_INJECTOR_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/injector.hpp"

    #include <boost/mpl/if.hpp>
    #include <boost/mpl/has_xxx.hpp>

    namespace boost {
    namespace di {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(scope)

    inline injector<> make_injector() {
        return injector<>();
    }

    #define BOOST_PP_FILENAME_1 "boost/di/make_injector.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_TYPES_MPL_LIMIT_FROM(1)
    #include BOOST_PP_ITERATE()

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(TArgs)>
    injector<typename detail::concepts<BOOST_DI_MPL_VECTOR_TYPES_PASS(TArgs)>::type>
    inline make_injector(BOOST_DI_ARGS(TArgs, args)) {
        return injector<
            typename detail::concepts<
                BOOST_DI_MPL_VECTOR_TYPES_PASS(TArgs)
              , aux::mpl::if_<
                    has_scope<aux::mpl::_2>
                  , detail::default_scope<aux::mpl::_2>
                  , aux::mpl::_2 // argument not supported
                >
            >::type
        >(BOOST_DI_ARGS_PASS(args));
    }

#endif

