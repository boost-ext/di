//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_SINGLETON_HPP
    #define BOOST_DI_SCOPES_SINGLETON_HPP

    #include <memory>
    #include <cassert>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/shared_ptr.hpp>
    #include <boost/make_shared.hpp>
    #include <boost/mpl/assert.hpp>

    #include "boost/di/type_traits/has_traits.hpp"
    #include "boost/di/named.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (       \
        BOOST_DI_ITERATION_PARAMS(              \
            1                                   \
          , BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE  \
          , "boost/di/scopes/singleton.hpp"     \
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
    class singleton
    {
    public:
        template<
            typename TExpected
          , typename TGiven = TExpected
        >
        class scope
        {
            class empty_deleter
            {
            public:
                void operator()(void*) { }
            };

        public:
            typedef scope result_type;

            template<typename I, typename TName>
            operator named<I, TName>() const {
                return object_;
            }

            template<typename I>
            operator shared_ptr<I>() const {
                return object_;
            }

            template<typename I, typename TName>
            operator named<shared_ptr<I>, TName>() const {
                return object_;
            }

        #if !defined(BOOST_NO_CXX11_SMART_PTR)
            template<typename I>
            operator std::shared_ptr<I>() const {
                return std::shared_ptr<I>(object_.get(), empty_deleter());
            }

            template<typename I, typename TName>
            operator named<std::shared_ptr<I>, TName>() const {
                return named<std::shared_ptr<I>, TName>(std::shared_ptr<I>(object_.get(), empty_deleter()));
            }
        #endif

            result_type& create() {
                return create_impl<TGiven>();
            }

            template<typename T>
            result_type& create_impl(typename disable_if<BOOST_PP_CAT(type_traits::has_, BOOST_DI_CREATE)<T> >::type* = 0) {
                if (!object_) {
                    object_.reset(new T());
                }

                return *this;
            }

            template<typename T>
            result_type& create_impl(typename enable_if<BOOST_PP_CAT(type_traits::has_, BOOST_DI_CREATE)<T> >::type* = 0) {
                if (!object_) {
                    object_.reset(T().BOOST_DI_CREATE());
                }

                return *this;
            }

            #include BOOST_PP_ITERATE()

        private:
            shared_ptr<TExpected> object_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type& create(BOOST_DI_ARGS(Args, args)) {
        return create_impl<TGiven>(BOOST_DI_ARGS_FORWARD(args));
    }

    template<typename T, BOOST_DI_TYPES(Args)>
    result_type& create_impl(BOOST_DI_ARGS(Args, args), typename disable_if<BOOST_PP_CAT(type_traits::has_, BOOST_DI_CREATE)<T> >::type* = 0) {
        if (!object_) {
            object_.reset(new T(BOOST_DI_ARGS_FORWARD(args)));
        }

        return *this;
    }

    template<typename T, BOOST_DI_TYPES(Args)>
    result_type& create_impl(BOOST_DI_ARGS(Args, args), typename enable_if<BOOST_PP_CAT(type_traits::has_, BOOST_DI_CREATE)<T> >::type* = 0) {
        if (!object_) {
            object_.reset(T().BOOST_DI_CREATE(BOOST_DI_ARGS_FORWARD(args)));
        }

        return *this;
    }

#endif

