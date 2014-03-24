//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_CREATOR_HPP
    #define BOOST_DI_DETAIL_CREATOR_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"
    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/type_traits/make_plain.hpp"
    #include "boost/di/type_traits/is_same_base_of.hpp"
    #include "boost/di/detail/binder.hpp"
    #include "boost/di/detail/builder.hpp"

    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/push_back.hpp>

    namespace boost {
    namespace di {
    namespace detail {

    template<
        typename TDependecies
      , template<typename, typename = builder> class TBinder = binder
    >
    class creator
    {
        template<typename T, typename TCallStack>
        struct binder
            : TBinder<TDependecies>::template resolve<T, TCallStack>::type
        { };

        template<typename TDependency>
        struct ctor
            : type_traits::ctor_traits<typename TDependency::given>::type
        { };

        template<
            typename T
          , typename TCallStack
          , typename TDependency
        >
        struct dependency
        {
            typedef T type;
            typedef TCallStack call_stack;
            typedef typename TDependency::given given;
            typedef typename TDependency::expected expected;
            typedef typename TDependency::scope scope;
        };

        template<
            typename T
          , typename TCallStack
          , typename TPolicies
          , typename TDeps
          , typename TRefs
          , typename TVisitor
        >
        class eager_creator
        {
            typedef typename type_traits::make_plain<T>::type plain_t;

        public:
            eager_creator(creator& c, TDeps& deps, TRefs& refs, const TVisitor& visitor)
                : c_(c), deps_(deps), refs_(refs), visitor_(visitor)
            { }

            template<
                typename U
                BOOST_DI_FEATURE(3, FUNCTION_TEMPLATE_DEFAULT_ARGS)(
                    BOOST_DI_COMMA()
                    typename PU = typename type_traits::make_plain<U>::type
                  , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
                )
            >
            operator U() {
                BOOST_DI_FEATURE(1, NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)(
                    typedef typename type_traits::make_plain<U>::type PU;
                )

                return c_.create_impl<
                    U
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<U, TCallStack>
                >(deps_, refs_, visitor_);
            }

            template<
                typename U
                BOOST_DI_FEATURE(3, FUNCTION_TEMPLATE_DEFAULT_ARGS)(
                    BOOST_DI_COMMA()
                    typename PU = typename type_traits::make_plain<U>::type
                  , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
                )
            >
            operator const U&() const {
                BOOST_DI_FEATURE(1, NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)(
                    typedef typename type_traits::make_plain<U>::type PU;
                )

                return c_.create_impl<
                    const U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<const U&, TCallStack>
                >(deps_, refs_, visitor_);
            }

            template<
                typename U
                BOOST_DI_FEATURE(3, FUNCTION_TEMPLATE_DEFAULT_ARGS)(
                    BOOST_DI_COMMA()
                    typename PU = typename type_traits::make_plain<U>::type
                  , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
                )
            >
            operator U&() const {
                BOOST_DI_FEATURE(1, NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)(
                    typedef typename type_traits::make_plain<U>::type PU;
                )

                return c_.create_impl<
                    U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<U&, TCallStack>
                >(deps_, refs_, visitor_);
            }

        private:
            creator& c_;
            TDeps& deps_;
            TRefs& refs_;
            const TVisitor& visitor_;
        };

    public:
        explicit creator(TBinder<TDependecies> binder = TBinder<TDependecies>())
            : binder_(binder)
        { }

        template<
            typename T
          , typename TParent // to ignore copy/move ctor
          , typename TCallStack
          , typename TPolicies
          , typename TDeps
          , typename TRefs
          , typename TVisitor
        >
        eager_creator<TParent, TCallStack, TPolicies, TDeps, TRefs, TVisitor>
        create(TDeps& deps, TRefs& refs, const TVisitor& visitor
              , typename enable_if<is_same<T, any_type> >::type* = 0) {
            return eager_creator<TParent, TCallStack, TPolicies, TDeps, TRefs, TVisitor>(
                *this, deps, refs, visitor
            );
        }

        template<
            typename T
          , typename // TParent - not needed
          , typename TCallStack
          , typename TPolicies
          , typename TDeps
          , typename TRefs
          , typename TVisitor
        >
        const convertibles::convertible<T>& create(
            TDeps& deps, TRefs& refs, const TVisitor& visitor, typename disable_if<is_same<T, any_type> >::type* = 0) {
            return create_impl<
                T
              , typename mpl::push_back<
                    TCallStack
                  , typename binder<T, TCallStack>::given
                >::type
              , TPolicies
              , binder<T, TCallStack>
            >(deps, refs, visitor);
        }

    private:
        #define BOOST_PP_FILENAME_1 "boost/di/detail/creator.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
        #include BOOST_PP_ITERATE()

        template<typename TSeq, typename T>
        typename enable_if<mpl::empty<TSeq> >::type assert_policies() { }

        template<typename TSeq, typename T>
        typename disable_if<mpl::empty<TSeq> >::type assert_policies() {
            typedef typename mpl::front<TSeq>::type policy;
            policy::template assert_policy<T>();
            assert_policies<typename mpl::pop_front<TSeq>::type, T>();
        }

        TBinder<TDependecies> binder_;
    };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TRefs
      , typename TVisitor
    >
    typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == BOOST_PP_ITERATION()
      , const convertibles::convertible<T>&
    >::type create_impl(TDeps& deps, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, dependency_type>();
        (visitor)(dependency_type());

        return binder_.template resolve_impl<
            T
          , typename ctor<TDependency>::type
          , TCallStack
          , TPolicies
          , TDependency
        >(*this, deps, refs, visitor);
    }

#endif

