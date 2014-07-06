//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_AUX_DETAIL_BIND_HPP
    #define BOOST_DI_AUX_DETAIL_BIND_HPP

    #include "boost/di/aux_/config.hpp"

    #include <boost/preprocessor/cat.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>

    namespace boost {
    namespace di {
    namespace aux {
    namespace detail {

    #define BOOST_PP_FILENAME_1 "boost/di/aux_/detail/bind.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
    #include BOOST_PP_ITERATE()

    } // namespace detail
    } // namespace aux
    } // namespace di
    } // namespace boost

    #endif

#else

    #define n BOOST_PP_ITERATION()

    template<
        typename R
      , typename T
        BOOST_PP_COMMA_IF(n)
        BOOST_DI_TYPES(Args)
    >
    class BOOST_PP_CAT(bind, n)
    {
        typedef R(T::*f_t)(BOOST_DI_ARGS_NOT_USED(Args)) const;

    public:
        #define BOOST_DI_bind_ARGS_PASS(z, n, na) \
            BOOST_PP_COMMA_IF(n) args##n(args##n)

        BOOST_PP_CAT(bind, n)(
            const f_t& f
          , const T& object
            BOOST_PP_COMMA_IF(n)
            BOOST_DI_ARGS(Args, args)
        ) : f_(f)
          , object_(object)
            BOOST_PP_COMMA_IF(n)
            BOOST_PP_REPEAT(n, BOOST_DI_bind_ARGS_PASS, ~)
        { }

        #undef BOOST_DI_bind_ARGS_PASS

        R operator()() const { return (object_.*f_)(BOOST_DI_ARGS_PASS(args)); }

    private:
        T object_;
        f_t f_;

        #define BOOST_DI_bind_MEMBER_DECL(z, n, na) Args##n args##n;
        BOOST_PP_REPEAT(n, BOOST_DI_bind_MEMBER_DECL, ~)
        #undef BOOST_DI_bind_MEMBER_DECL
    };

    #undef n

#endif

