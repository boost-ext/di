//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_TYPE_TRAITS_HAS_CTOR_HPP
    #define BOOST_DI_TYPE_TRAITS_HAS_CTOR_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/core/any_type.hpp"

    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>
    #include <boost/mpl/x11/vector.hpp>

    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename, typename>
    class has_ctor;

    #define BOOST_PP_FILENAME_1 "boost/di/type_traits/has_ctor.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(2)
    #include BOOST_PP_ITERATE()

    } // namespace type_traits
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename T, BOOST_DI_TYPES(TArgs)>
    class has_ctor<T, mpl::x11::vector<BOOST_DI_TYPES_PASS(TArgs)> >
    {
    public:
        template<typename U>
        static mpl::aux::yes_tag test(BOOST_DI_FEATURE_DECLTYPE(U(TArgs()...))*);

        template<typename>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

#endif

