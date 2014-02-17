//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_HAS_CTOR_HPP
#define BOOST_DI_TYPE_TRAITS_HAS_CTOR_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/preprocessed/type_traits/make_plain.hpp"
#include "boost/di/aux_/preprocessed/type_traits/is_same_base_of.hpp"
#include <boost/config.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/aux_/yes_no.hpp>

    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename, typename>
    class has_ctor;

        template<typename T>
        class has_ctor<T, mpl::int_<1> >
        {
            struct any_type
            {
                typedef typename type_traits::make_plain<T>::type plain_t;

                template<
                    typename U
                  , typename PU = typename type_traits::make_plain<U>::type
                  , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
                >
                operator U() const;
            };

            template<typename U>
            static mpl::aux::yes_tag test(BOOST_TYPEOF_TPL(U(any_type()))*);

            template<typename U>
            static mpl::aux::no_tag test(...);

        public:
            typedef has_ctor type;
            BOOST_STATIC_CONSTANT(
                bool
              , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
            );
        };

    template<typename T>
    class has_ctor<T, mpl::int_<2> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<3> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<4> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<5> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<6> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<7> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<8> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<9> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<10> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<11> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<12> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<13> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<14> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<15> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<16> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<17> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<18> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<19> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T>
    class has_ctor<T, mpl::int_<20> >
    {
        struct any_type
        {
            template<typename U> operator U&() const;

            template<typename U> operator U();
        };

        template<typename U>
        static mpl::aux::yes_tag test(
            BOOST_TYPEOF_TPL(U(
                any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() , any_type() )
            )*
        );

        template<typename U>
        static mpl::aux::no_tag test(...);

    public:
        typedef has_ctor type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    }
    }
    }

#endif

