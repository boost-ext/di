//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_PER_REQUEST_HPP
    #define BOOST_DI_SCOPES_PER_REQUEST_HPP

    #include <string>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/shared_ptr.hpp>
    #include <boost/make_shared.hpp>
    #include <boost/function.hpp>
    #include <boost/bind.hpp>
    #include <boost/non_type.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/string.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>
    #include <boost/mpl/void.hpp>

    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/named.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (       \
        BOOST_DI_ITERATION_PARAMS(              \
            1                                   \
          , BOOST_DI_LIMIT_SIZE                 \
          , "boost/di/scopes/per_request.hpp"   \
        )                                       \
    )

    namespace boost {
    namespace di {
    namespace scopes {

    template<
        template<
            typename
        > class TAllocator = std::allocator
    >
    class per_request
    {
        template<typename T>
        class has_value
        {
            struct helper { static int value; };
            struct base
                : helper
                , mpl::if_<
                      is_arithmetic<T>
                    , mpl::void_
                    , T
                  >::type
            { };

            template<typename C> static mpl::aux::no_tag  test(non_type<const int*, &C::value>*);
            template<typename>   static mpl::aux::yes_tag test(...);

        public:
            BOOST_STATIC_CONSTANT(
                bool
              , value = sizeof(test<base>(0)) == sizeof(mpl::aux::yes_tag)
            );
        };

        template<
            typename
          , typename = void
        >
        class explicit_impl
            : public mpl::false_
        { };

        template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, int C)>
        class explicit_impl<
            mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)>
        >
            : public mpl::true_
        {
        public:
            inline static std::string create() {
                return mpl::c_str<
                    mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)>
                >::value;
            }
        };

        template<
            typename T
        >
        class explicit_impl<T, typename enable_if<has_value<T> >::type>
            : public mpl::true_
        {
        public:
            inline static BOOST_TYPEOF_TPL(T::value) create() {
                return T::value;
            }
        };

    public:
        template<
            typename TExpected
          , typename TGiven = TExpected
          , typename = void
        >
        class scope
        {
        public:
            typedef scope result_type;

            operator TGiven*() const {
                return object_();
            }

            operator TGiven() const {
                return *object_(); //TODO delete
            }

            template<typename I, typename TName>
            operator named<I, TName>() const {
                return *object_(); //TODO delete
            }

            template<typename I>
            operator shared_ptr<I>() const {
                return shared_ptr<I>(object_());
            }

            template<typename I, typename TName>
            operator named<shared_ptr<I>, TName>() const {
                return shared_ptr<I>(object_());
            }

            template<typename I>
            operator std::auto_ptr<I>() const {
                return std::auto_ptr<I>(object_());
            }

            template<typename I, typename TName>
            operator named<std::auto_ptr<I>, TName>() const {
                return std::auto_ptr<I>(object_());
            }

        #if !defined(BOOST_NO_CXX11_SMART_PTR)
            template<typename I>
            operator std::shared_ptr<I>() const {
                return object_();
            }

            template<typename I, typename TName>
            operator named<std::shared_ptr<I>, TName>() const {
                return named<std::shared_ptr<I>, TName>(object_());
            }

            template<typename I>
            operator std::unique_ptr<I>() const {
                return std::unique_ptr<I>(object_());
            }

            template<typename I, typename TName>
            operator named<std::unique_ptr<I>, TName>() const {
                return named<std::unique_ptr<I>, TName>(object_());
            }
        #endif

            result_type& create() {
                object_ = bind(static_cast<TGiven*(*)()>(&scope::create_impl));
                return *this;
            }

            static TGiven* create_impl() {
                return type_traits::create_traits<TGiven, TGiven>();
            }

            #include BOOST_PP_ITERATE()

        private:
            function<TGiven*()> object_;
        };

        template<
            typename TExpected
          , typename TGiven
        >
        class scope<
            TExpected
          , TGiven
          , typename enable_if<explicit_impl<TGiven> >::type
        >
        {
            typedef BOOST_TYPEOF_TPL(explicit_impl<TGiven>::create()) T;

        public:
            typedef scope result_type;

            operator T() const {
                return object_();
            }

            template<typename TName>
            operator named<T, TName>() const {
                return object_();
            }

            template<typename TName>
            operator shared_ptr<T>() const {
                return make_shared<T>(object_());
            }

            template<typename TName>
            operator named<shared_ptr<T>, TName>() const {
                return make_shared<T>(object_());
            }

        #if !defined(BOOST_NO_CXX11_SMART_PTR)
            template<typename TName>
            operator std::shared_ptr<T>() const {
                return std::make_shared<T>(object_());
            }

            template<typename TName>
            operator named<std::shared_ptr<T>, TName>() const {
                return std::make_shared<T>(object_());
            }
        #endif

            result_type& create() {
                object_ = bind(&explicit_impl<TGiven>::create);
                return *this;
            }

        private:
            function<T()> object_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type& create(BOOST_DI_ARGS(Args, args)) {
        object_ = bind(
            &scope::create_impl<BOOST_DI_TYPES_PASS(Args)>
          , BOOST_DI_ARGS_FORWARD(args)
        );
        return *this;
    }

    template<BOOST_DI_TYPES(Args)>
    static TGiven* create_impl(BOOST_DI_ARGS(Args, args)) {
        return type_traits::create_traits<TGiven, TGiven>(BOOST_DI_ARGS_FORWARD(args));
    }

#endif

