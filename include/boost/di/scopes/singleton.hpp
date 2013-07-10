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
    #include <boost/shared_ptr.hpp>
    #include <boost/make_shared.hpp>
    #include <boost/mpl/assert.hpp>

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

    template<typename T>
    class convertible_shared
    {
    public:
        convertible_shared(T* object = NULL) // non explicit
            : object_(object)
        { }

        template<typename I>
        operator shared_ptr<I>() const {
            return object_;
        }

        template<typename I, typename TName>
        operator named<I, TName>() const {
            return object_;
        }

        template<typename I, typename TName>
        operator named<shared_ptr<I>, TName>() const {
            return named<shared_ptr<I>, TName>(object_);
        }

        operator T() const {
            BOOST_MPL_ASSERT_MSG(
                false
              , SINGLETON_SCOPE_CANT_BE_PASSED_BY_COPY
              , (T)
            );
        }

        operator bool() const {
            return object_.get();
        }

    private:
        shared_ptr<T> object_;
    };

    } // namespace aux

    template<
        template<
            typename
        > class TAllocator = std::allocator
      , template<
            typename
        > class TConvertible = aux::convertible_shared
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
            typedef TConvertible<TGiven> result_type;

            result_type create() {
                if (!object_) {
                    object_ = new TGiven(); //TODO allocator
                }

                return object_;
            }

            #include BOOST_PP_ITERATE()

        private:
            result_type object_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        if (!object_) {
            object_ = new TGiven(BOOST_DI_ARGS_FORWARD(args)); //TODO allocator
        }

        return object_;
    }

#endif

