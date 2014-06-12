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
    #include "boost/di/wrappers/universal.hpp"
    #include "boost/di/detail/binder.hpp"
    #include "boost/di/detail/builder.hpp"

    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/push_back.hpp>

    namespace boost {
    namespace di {
    namespace detail {

    template<
        typename TDependecies = mpl::vector0<>
      , template<
            typename
          , typename = ::boost::di::detail::builder
        > class TBinder = binder
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
        struct created
        {
            typedef T type;
            typedef TCallStack call_stack;
            typedef TDependency dependency;
        };

        template<
            typename T
          , typename TCallStack
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        class eager_creator
        {
            eager_creator& operator=(const eager_creator&);

        public:
            eager_creator(creator& c, TDeps& deps, TRefs& refs, const TVisitor& visitor, const TPolicies& policies)
                : c_(c), deps_(deps), refs_(refs), visitor_(visitor), policies_(policies)
            { }

            template<
                typename U
                BOOST_DI_FEATURE(FUNCTION_TEMPLATE_DEFAULT_ARGS)(
                    , typename = typename disable_if<
                        type_traits::is_same_base_of<
                            typename type_traits::make_plain<U>::type
                          , typename type_traits::make_plain<T>::type
                        >
                    >::type
                )
            >
            operator const U&() const {
                return c_.create_impl<
                    const U&
                    BOOST_DI_FEATURE_EXAMINE_CALL_STACK(
                      , typename mpl::push_back<
                            TCallStack
                          , const U&
                        >::type
                    )
                    BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(
                      , TCallStack
                    )
                  , binder<const U&, TCallStack>
                >(deps_, refs_, visitor_, policies_);
            }

            template<
                typename U
                BOOST_DI_FEATURE(FUNCTION_TEMPLATE_DEFAULT_ARGS)(
                    , typename = typename disable_if<
                        type_traits::is_same_base_of<
                            typename type_traits::make_plain<U>::type
                          , typename type_traits::make_plain<T>::type
                        >
                    >::type
                )
            >
            operator U&() const {
                return c_.create_impl<
                    U&
                    BOOST_DI_FEATURE_EXAMINE_CALL_STACK(
                      , typename mpl::push_back<
                            TCallStack
                          , U&
                        >::type
                    )
                    BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(
                      , TCallStack
                    )
                  , binder<U&, TCallStack>
                >(deps_, refs_, visitor_, policies_);
            }

            template<typename U>
            operator aux::auto_ptr<U>&() {
                return c_.create_impl<
                    aux::auto_ptr<U>
                    BOOST_DI_FEATURE_EXAMINE_CALL_STACK(
                      , typename mpl::push_back<
                            TCallStack
                          , aux::auto_ptr<U>
                        >::type
                    )
                    BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(
                      , TCallStack
                    )
                  , binder<aux::auto_ptr<U>, TCallStack>
                >(deps_, refs_, visitor_, policies_);
            }

            BOOST_DI_WKND(MSVC)(
                template<typename U>
                operator aux::unique_ptr<U>() {
                    return c_.create_impl<
                        aux::unique_ptr<U>
                        BOOST_DI_FEATURE_EXAMINE_CALL_STACK(
                          , typename mpl::push_back<
                                TCallStack
                              , aux::unique_ptr<U>
                            >::type
                        )
                        BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(
                          , TCallStack
                        )
                      , binder<aux::unique_ptr<U>, TCallStack>
                    >(deps_, refs_, visitor_, policies_);
                }
            )

            BOOST_DI_WKND(NO_MSVC)(
                template<
                    typename U
                    BOOST_DI_FEATURE(FUNCTION_TEMPLATE_DEFAULT_ARGS)(
                        , typename = typename disable_if<
                            type_traits::is_same_base_of<
                                typename type_traits::make_plain<U>::type
                              , typename type_traits::make_plain<T>::type
                            >
                        >::type
                    )
                >
                operator U() {
                    return c_.create_impl<
                        U
                        BOOST_DI_FEATURE_EXAMINE_CALL_STACK(
                          , typename mpl::push_back<
                                TCallStack
                              , U
                            >::type
                        )
                        BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(
                          , TCallStack
                        )
                      , binder<U, TCallStack>
                    >(deps_, refs_, visitor_, policies_);
                }
            )

        private:
            creator& c_;
            TDeps& deps_;
            TRefs& refs_;
            const TVisitor& visitor_;
            const TPolicies& policies_;
        };

    public:
        explicit creator(const TBinder<TDependecies>& binder = TBinder<TDependecies>())
            : binder_(binder)
        { }

        template<
            typename T
          , typename TParent // to ignore copy/move ctor
          , typename TCallStack
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        eager_creator<TParent, TCallStack, TDeps, TRefs, TVisitor, TPolicies>
        create(TDeps& deps, TRefs& refs, const TVisitor& visitor, const TPolicies& policies
             , typename enable_if<is_same<T, any_type> >::type* = 0) {
            return eager_creator<TParent, TCallStack, TDeps, TRefs, TVisitor, TPolicies>(
                *this, deps, refs, visitor, policies
            );
        }

        template<
            typename T
          , typename // TParent - not needed
          , typename TCallStack
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        wrappers::universal<T> create(
            TDeps& deps
          , TRefs& refs
          , const TVisitor& visitor
          , const TPolicies& policies
          , typename disable_if<is_same<T, any_type> >::type* = 0) {
            return create_impl<
                T
                BOOST_DI_FEATURE_EXAMINE_CALL_STACK(
                  , typename mpl::push_back<
                        TCallStack
                      , T
                    >::type
                )
                BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(
                  , TCallStack
                )
              , binder<T, TCallStack>
            >(deps, refs, visitor, policies);
        }

    private:
        #define BOOST_PP_FILENAME_1 "boost/di/detail/creator.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
        #include BOOST_PP_ITERATE()

        template<typename TSeq, typename T, typename TPolicies>
        static typename enable_if<mpl::empty<TSeq> >::type assert_policies(const TPolicies&) { }

        template<typename TSeq, typename T, typename TPolicies>
        static typename disable_if<mpl::empty<TSeq> >::type assert_policies(const TPolicies& policies) {
            typedef typename mpl::front<TSeq>::type policy;
            static_cast<const policy&>(policies).template assert_policy<T>();
            assert_policies<typename mpl::pop_front<TSeq>::type, T>(policies);
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
      , typename TDependency
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == BOOST_PP_ITERATION()
      , wrappers::universal<T>
    >::type create_impl(TDeps& deps, TRefs& refs, const TVisitor& visitor, const TPolicies& policies) {
        typedef created<T, TCallStack, TDependency> created_type;
        assert_policies<typename TPolicies::types, created_type>(policies);
        (visitor)(created_type());

        return binder_.template resolve_impl<
            T
          , typename ctor<TDependency>::type
          , TCallStack
          , TDependency
        >(*this, deps, refs, visitor, policies);
    }

#endif

