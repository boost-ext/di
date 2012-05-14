//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_AUX_VISITOR_HPP
    #define BOOST_DI_AUX_VISITOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include "boost/di/aux/make_plain.hpp"
    #include "boost/di/aux/dependency.hpp"
    #include "boost/di/aux/ctor_traits.hpp"
    #include "boost/di/detail/binder.hpp"
    #include "boost/di/scopes/per_request.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (       \
        BOOST_DI_ITERATION_PARAMS(              \
            0                                   \
          , BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE  \
          , "boost/di/aux_/visitor.hpp"         \
        )                                       \
    )

    namespace boost {
    namespace di {
    namespace aux_ {

    template<
        typename TDeps
      , template<
            typename
          , typename
          , typename = TDeps
          , typename = aux_::dependency<scopes::per_request, mpl::_1, mpl::_2>
        > class TBinder = binder
      , template<
            typename
          , typename = void
        > class TCtorTraits = aux_::ctor_traits
    >
    class visitor_impl
    {
        template<typename TDependency>
        struct ctor
            : TCtorTraits<typename TDependency::given>::type
        { };

        template<
            typename T
          , typename TCallStack
          , typename TDependency
        >
        struct dependency
        {
            typedef T type;
            typedef TCallStack context;
            typedef typename TDependency::given given;
            typedef typename TDependency::expected expected;
            typedef typename TDependency::scope scope;
        };

    public:
        template<
            typename T
          , typename TCallStack
          , typename TVisitor
        >
        static void execute(const TVisitor& visitor) {
            typedef typename TBinder<T, TCallStack>::type to_bo_created_t;
            execute_impl<
                T
              , to_bo_created_t
              , typename mpl::push_back<TCallStack, typename to_bo_created_t::given>::type
            >(visitor);
        }

    private:
        #include BOOST_PP_ITERATE()
    };

    template<typename TDeps>
    struct visitor
        : visitor_impl<TDeps>
    { };

    } // namespace aux_
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value
        ==
        BOOST_PP_ITERATION()
    >::type execute_impl(const TVisitor& visitor) {

        visitor.template operator()<dependency<T, TCallStack, TDependency> >();

        #define BOOST_DI_VISITOR_EXECUTE(z, n, _)                               \
            execute<                                                            \
                typename mpl::at_c<typename ctor<TDependency>::type, n>::type   \
              , TCallStack                                                      \
            >(visitor);

        BOOST_PP_REPEAT(
            BOOST_PP_ITERATION()
          , BOOST_DI_VISITOR_EXECUTE
          , ~
        );

        #undef BOOST_DI_VISITOR_EXECUTE
    }

#endif

