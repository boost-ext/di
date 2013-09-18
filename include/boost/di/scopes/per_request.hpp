//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_PER_REQUEST_HPP
    #define BOOST_DI_SCOPES_PER_REQUEST_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/bind.hpp>

    #include "boost/di/convertibles/convertible_copy.hpp"
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
        template<typename> class TConvertible = convertibles::convertible_copy
    >
    class per_request
    {
    public:
        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            result_type create() {
                return result_type(::boost::bind(
                    static_cast<TExpected*(*)()>(
                        &type_traits::create_traits<TExpected, TGiven>
                    )
                ));
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
        return result_type(::boost::bind(
            &type_traits::create_traits<TExpected, TGiven, BOOST_DI_TYPES_PASS(Args)>
          , BOOST_DI_ARGS_PASS(args)
        ));
    }

#endif

