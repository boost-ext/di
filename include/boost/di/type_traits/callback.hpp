//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_TYPE_TRAITS_CALL_BACK_HPP
    #define BOOST_DI_TYPE_TRAITS_CALL_BACK_HPP

    #include "boost/di/aux_/meta.hpp"

    #include <boost/type_traits/remove_reference.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>

    #define BOOST_PP_ITERATION_PARAMS_1 ( \
        BOOST_DI_ITERATION_PARAMS(1, "boost/di/type_traits/callback.hpp"))

    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename>
    class callback;

    template<typename R>
    class callback<R(*)()>
    {
        typedef R(*f_t)();

    public:
        explicit callback(const f_t& f) : f(f) { }
        R operator()() const { return f(); }

    private:
        f_t f;
    };

    #include BOOST_PP_ITERATE()

    template<typename F>
    callback<F> make_callback(const F& f) {
        return callback<F>(f);
    }

    } // namespace type_traits
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename R, BOOST_DI_TYPES(Args)>
    class callback<R(*)(BOOST_DI_TYPES_PASS(Args))>
    {
        typedef R(*f_t)(BOOST_DI_TYPES_PASS(Args));

    public:
        #define BOOST_DI_CALLBACK_ARGS_PASS(z, n, na) \
            BOOST_PP_COMMA_IF(n) args##n(args##n)

        callback(const f_t& f, BOOST_DI_ARGS(Args, args))
            : f(f), BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_CALLBACK_ARGS_PASS, ~)
        { }

        #undef BOOST_DI_CALLBACK_ARGS_PASS

        R operator()() const { return f(BOOST_DI_ARGS_PASS(args)); }

    private:
        f_t f;

        #define BOOST_DI_CALLBACK_MEMBER_DECL(z, n, na) \
            typename boost::remove_reference<Args##n>::type args##n;
        BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_CALLBACK_MEMBER_DECL, ~)
        #undef BOOST_DI_CALLBACK_MEMBER_DECL
    };

    template<typename F, BOOST_DI_TYPES(Args)>
    callback<F> make_callback(const F& f, BOOST_DI_ARGS(Args, args)) {
        return callback<F>(f, BOOST_DI_ARGS_PASS(args));
    }

#endif

