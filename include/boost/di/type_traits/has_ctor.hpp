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
    #include "boost/di/type_traits/make_plain.hpp"
    #include "boost/di/type_traits/is_same_base_of.hpp"

    #include <memory>
    #include <boost/config.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>

    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename, typename>
    class has_ctor;

    BOOST_DI_FEATURE(FUNCTION_TEMPLATE_DEFAULT_ARGS)(
        template<typename T>
        class has_ctor<T, mpl::int_<1> >
        {
            class any_type
            {
            public:
                template<
                    typename U
                  , typename = typename disable_if<
                        is_same_base_of<
                            typename make_plain<U>::type
                          , typename make_plain<T>::type
                        >
                    >::type
                >
                operator U();

                template<typename U>
                operator aux::auto_ptr<U>&();
            };

            template<typename U>
            static mpl::aux::yes_tag test(BOOST_DI_FEATURE_DECLTYPE(U(any_type()))*);

            template<typename>
            static mpl::aux::no_tag test(...);

        public:
            typedef has_ctor type;

            BOOST_STATIC_CONSTANT(
                bool
              , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
            );
        };
    )

    BOOST_DI_FEATURE(NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)(
        template<typename T>
        class has_ctor<T, mpl::int_<1> > { public:
            BOOST_STATIC_CONSTANT(bool, value = false);
        };
    )

    #define BOOST_PP_FILENAME_1 "boost/di/type_traits/has_ctor.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(2)
    #include BOOST_PP_ITERATE()

    } // namespace type_traits
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename T>
    class has_ctor<T, mpl::int_<BOOST_PP_ITERATION()> >
    {
        class any_type
        {
        public:
            template<typename U> operator const U&() const;
            template<typename U> operator U&() const;
            template<typename U> operator aux::auto_ptr<U>&();
            BOOST_DI_FEATURE(RVALUE_REFERENCES)(
                template<typename U> operator U&&() const;
            )
            BOOST_DI_WKND(NO_MSVC)(
                template<typename U> operator U();
            )
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_DI_FEATURE_DECLTYPE(U(
                BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), any_type() BOOST_PP_INTERCEPT))
            )*
        );

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

