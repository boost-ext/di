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
    #include <boost/make_shared.hpp>

    #include "boost/di/named.hpp"
    #include "boost/di/config.hpp"

//TODO
#include "boost/di/scopes/external.hpp"

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

    namespace aux {

    template<typename TExpected>
    class convertible_per_request
    {
    public:
        template<typename T>
        convertible_per_request(const shared_ptr<T>& object) // non explicit
            : object_(object)
        { }

        template<typename T>
        operator shared_ptr<T>() const {
            return object_;
        }

        operator TExpected*() const {
            return object_.get();
        }

        operator TExpected&() const {
            return *object_;
        }

        template<typename TName>
        operator named<shared_ptr<TExpected>, TName>() const {
            return object_;
        }

    private:
        shared_ptr<TExpected> object_;
    };

    } // namespace aux

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
            //typedef aux::convertible_per_request<TExpected> result_type;
            typedef external<TExpected> result_type;

            result_type create() {
                return allocate_shared<TGiven>(TAllocator<TGiven>());
            }

            #include BOOST_PP_ITERATE()
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        return allocate_shared<TGiven>(
            TAllocator<TGiven>()
          , BOOST_DI_ARGS_FORWARD(args)
        );
    }

#endif

