//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_VISITOR_HPP
    #define BOOST_DI_DETAIL_VISITOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>

    #include "boost/di/config.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            0                               \
          , BOOST_DI_LIMIT_SIZE             \
          , "boost/di/detail/visitor.hpp"   \
        )                                   \
    )

    namespace boost {
    namespace di {
    namespace detail {

    template<
        typename TBinder
      , template<
            typename
          , typename = void
        > class TCtorTraits = type_traits::ctor_traits
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
        >
        struct binder
            : TBinder::template get_dependency<T, TCallStack>::type
        { };

        template<
            typename T
          , typename TCallStack
          , typename TDependency
        >
        struct dependency_impl
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
            execute_impl<
                T
              , typename mpl::push_back<
                    TCallStack
                  , typename binder<T, TCallStack>::given
                >::type
              , binder<T, TCallStack>
            >(visitor);
        }

    private:
        #include BOOST_PP_ITERATE()
    };

    template<typename TBinder>
    struct visitor
        : visitor_impl<TBinder>
    { };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename TCallStack
      , typename TDependency
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value
        ==
        BOOST_PP_ITERATION()
    >::type execute_impl(const TVisitor& visitor) {

        visitor.BOOST_DI_TEMPLATE_QUALIFIER
            operator()<dependency_impl<T, TCallStack, TDependency> >();

        #define BOOST_DI_VISITOR_EXECUTE(z, n, _)       \
            execute<                                    \
                typename mpl::at_c<                     \
                    typename ctor<TDependency>::type    \
                  , n                                   \
                >::type                                 \
              , TCallStack                              \
            >(visitor);

        BOOST_PP_REPEAT(
            BOOST_PP_ITERATION()
          , BOOST_DI_VISITOR_EXECUTE
          , ~
        );

        #undef BOOST_DI_VISITOR_EXECUTE
    }

#endif

