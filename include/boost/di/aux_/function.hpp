//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_AUX_FUNCTION_HPP
    #define BOOST_DI_AUX_FUNCTION_HPP

    #include <boost/di/aux_/config.hpp>
    #include <boost/di/aux_/memory.hpp>

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include <boost/preprocessor/arithmetic/sub.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/preprocessor/cat.hpp>

    namespace boost {
    namespace di {
    namespace aux {

    template<typename R>
    class ifunction
    {
    public:
        virtual ~ifunction() { }
        virtual R operator()() const = 0;
    };

    template<typename R>
    class function
    {
    public:
        function()
            : f_(0)
        { }

        template<typename T>
        function(const aux::shared_ptr<T>& f) // non explicit
            : f_(f)
        { }

        R operator()() const {
            return (*f_)();
        }

        aux::shared_ptr<ifunction<R> > f_;
    };

    #define BOOST_PP_FILENAME_1 "boost/di/aux_/function.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
    #include BOOST_PP_ITERATE()

    } // namespace aux_
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
        class BOOST_PP_CAT(bind_, n) : public ifunction<R>
        {
        public:
            #define BOOST_DI_bind_ARGS_PASS(z, n, na) \
                BOOST_PP_COMMA_IF(n) args##n(args##n)

            BOOST_PP_CAT(bind_, n)(
                const T& t
                BOOST_PP_COMMA_IF(n)
                BOOST_DI_ARGS(Args, args)
            ) : t(t)
                BOOST_PP_COMMA_IF(n)
                BOOST_PP_REPEAT(n, BOOST_DI_bind_ARGS_PASS, ~)
            { }

            #undef BOOST_DI_bind_ARGS_PASS

            R operator()() const { return t(BOOST_DI_ARGS_PASS(args)); }

        private:
            T t;

            #define BOOST_DI_bind_MEMBER_DECL(z, n, na) Args##n args##n;
            BOOST_PP_REPEAT(n, BOOST_DI_bind_MEMBER_DECL, ~)
            #undef BOOST_DI_bind_MEMBER_DECL
        };

        template<
            typename R
          , typename T
            BOOST_PP_COMMA_IF(n)
            BOOST_DI_TYPES(Args)
        >
        aux::shared_ptr<BOOST_PP_CAT(bind_, n)<R, T BOOST_PP_COMMA_IF(n) BOOST_DI_TYPES_PASS(Args)> >
        bind(const T& t BOOST_PP_COMMA_IF(n) BOOST_DI_ARGS(Args, args)) {
            return aux::shared_ptr<BOOST_PP_CAT(bind_, n)<R, T BOOST_PP_COMMA_IF(n) BOOST_DI_TYPES_PASS(Args)> >(new
                BOOST_PP_CAT(bind_, n)<R, T BOOST_PP_COMMA_IF(n) BOOST_DI_TYPES_PASS(Args)>(t BOOST_PP_COMMA_IF(n) BOOST_DI_ARGS_PASS(args))
            );
        }

    #undef n

#endif


