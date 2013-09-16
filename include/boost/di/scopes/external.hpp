//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
    #define BOOST_DI_SCOPES_EXTERNAL_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>

    #include "boost/di/convertibles/convertible_any.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_DI_LIMIT_SIZE             \
          , "boost/di/scopes/external.hpp"  \
        )                                   \
    )

    namespace boost {
    namespace di {
    namespace scopes {

    template<
        template<typename> class TConvertible = convertibles::convertible_any
    >
    class external
    {
    public:
        typedef void is_priority;

        template<
            typename TExpected
          , typename = TExpected
        >
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            template<typename T>
            explicit scope(const T& value)
                : object_(value)
            { }

            result_type create() {
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
    result_type create(BOOST_DI_ARGS_NOT_USED(Args)) {
        return object_;
    }

#endif

