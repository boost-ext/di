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

    template<typename T>
    class convertible_value
    {
    public:
        typedef T element_type;

        convertible_value(T* object) // non explicit
            : object_(object)
        { }

        operator T*() const {
            return object_; // has to be deleted by client
        }

        operator const T&() const {
            return *object_;
            //TODO should be deleted by di immediately and copied by client in ctor
        }

        //operator auto_ptr
        //operator unique_ptr

        template<typename I>
        operator shared_ptr<I>() const {
            return shared_ptr<I>(object_);
        }

        template<typename I, typename TName>
        operator named<I, TName>() const {
            return shared_ptr<I>(object_);
        }

        template<typename I, typename TName>
        operator named<shared_ptr<I>, TName>() const {
            return shared_ptr<I>(object_);
        }

    private:
        T* object_;
    };

    } // namespace aux

    template<
        template<
            typename
        > class TAllocator = std::allocator
      , template<
            typename
        > class TConvertible = aux::convertible_value
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
            typedef TConvertible<TGiven> result_type;

            result_type create() {
                return new TGiven(); //TODO allocator
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
        return new TGiven(BOOST_DI_ARGS_FORWARD(args)); //TODO allocator
    }

#endif

