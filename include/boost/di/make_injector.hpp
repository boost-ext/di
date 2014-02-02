//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_MAKE_INJECTOR_HPP
    #define BOOST_DI_MAKE_INJECTOR_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/injector.hpp"

    namespace boost {
    namespace di {

    inline injector<> make_injector() {
        return injector<>()();
    }

    #define BOOST_PP_FILENAME_1 "boost/di/make_injector.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_TYPES_MPL_LIMIT_FROM(1)
    #include BOOST_PP_ITERATE()

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    injector<typename detail::concepts<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >::type>
    inline make_injector(BOOST_DI_ARGS(Args, args)) {
        return injector<typename detail::concepts<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >::type>(
            BOOST_DI_ARGS_PASS(args)
        );
    }

#endif

