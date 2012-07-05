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
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/make_shared.hpp>
    #include <boost/shared_ptr.hpp>

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

    namespace aux {

    template<typename TExpected>
    class convertible_singleton
    {
    public:
        template<typename T>
        convertible_singleton(const shared_ptr<T>& object) // non explicit
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
    class singleton
    {
    public:
        template<
            typename TExpected
          , typename TGiven = TExpected
        >
        class scope
        {
        public:
            typedef aux::convertible_singleton<TExpected> result_type;

            result_type create() {
                if (!instance_) {
                    instance_ = allocate_shared<TGiven>(TAllocator<TGiven>());
                }

                return instance_;
            }

            #include BOOST_PP_ITERATE()

        private:
            shared_ptr<TGiven> instance_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        if (!instance_) {
            instance_ = allocate_shared<TGiven>(
                TAllocator<TGiven>()
              , BOOST_DI_ARGS_FORWARD(args)
            );
        }

        return instance_;
    }

#endif

