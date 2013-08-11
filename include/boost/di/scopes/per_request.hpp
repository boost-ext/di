//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_PER_REQUEST_HPP
    #define BOOST_DI_SCOPES_PER_REQUEST_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/shared_ptr.hpp>
    #include <boost/function.hpp>
    #include <boost/bind.hpp>

    #include "boost/di/named.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (       \
        BOOST_DI_ITERATION_PARAMS(              \
            1                                   \
          , BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE  \
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
    public:
        template<
            typename TExpected
          , typename TGiven = TExpected
        >
        class scope
        {
        public:
            typedef scope result_type;

            operator TGiven*() const {
                return object_();
            }

            operator const TGiven&() const {
                return *object_();
                //TODO should be deleted by di immediately and copied by client in ctor
            }

            template<typename I, typename TName>
            operator named<I, TName>() const {
                return *object_();
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

            static TGiven* create_impl() {
                return new TGiven();
            }

            result_type& create() {
                object_ = bind(static_cast<TGiven*(*)()>(&scope::create_impl));
                return *this;
            }

            #include BOOST_PP_ITERATE()

        private:
            function<TGiven*()> object_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type& create(BOOST_DI_ARGS(Args, args)) {
        object_ = bind(&scope::create_impl<BOOST_DI_TYPES_PASS(Args)>, BOOST_DI_ARGS_FORWARD(args));
        return *this;
    }

    template<BOOST_DI_TYPES(Args)>
    static TGiven* create_impl(BOOST_DI_ARGS(Args, args)) {
        return new TGiven(BOOST_DI_ARGS_FORWARD(args));
    }

#endif

