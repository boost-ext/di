//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_CORE_CREATOR_HPP
    #define BOOST_DI_CORE_CREATOR_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"
    #include "boost/di/wrappers/universal.hpp"
    #include "boost/di/core/binder.hpp"
    #include "boost/di/core/builder.hpp"
    #include "boost/di/core/any_type.hpp"

    #include <boost/none_t.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/push_back.hpp>

    namespace boost {
    namespace di {
    namespace core {

    template<
        typename TDependecies = mpl::vector0<>
      , template<
            typename
          , typename = ::boost::di::core::builder
        > class TBinder = binder
      , template<
            typename = none_t
          , typename = mpl::vector0<>
          , typename = none_t
          , typename = none_t
          , typename = none_t
          , typename = none_t
          , typename = none_t
        >
        class TAnyType = ::boost::di::core::any_type
    >
    class creator
    {
       template<
            typename
          , typename
          , typename
          , typename
          , typename
          , typename
          , typename
        > friend class TAnyType;

        template<typename TDependency>
        struct ctor
            : type_traits::ctor_traits<typename TDependency::given>::type
        { };

        template<typename T, typename TCallStack>
        struct resolve
            : TBinder<TDependecies>::template resolve<T, TCallStack>::type
        { };

    public:
        explicit creator(const TBinder<TDependecies>& binder = TBinder<TDependecies>())
            : binder_(binder)
        { }

        template<
            typename T
          , typename TParent // ignore copy/move ctor
          , typename TCallStack
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        TAnyType<TParent, TCallStack, creator, TDeps, TRefs, TVisitor, TPolicies>
        create(TDeps& deps, TRefs& refs, const TVisitor& visitor, const TPolicies& policies
             , typename enable_if<is_same<T, TAnyType<> > >::type* = 0) {
            return TAnyType<TParent, TCallStack, creator, TDeps, TRefs, TVisitor, TPolicies>(
                *this, deps, refs, visitor, policies
            );
        }

        template<
            typename T
          , typename // TParent - not used
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
          , typename disable_if<is_same<T, TAnyType<> > >::type* = 0) {
            return create_impl<T, TCallStack>(deps, refs, visitor, policies);
        }

    private:
        #define BOOST_PP_FILENAME_1 "boost/di/core/creator.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
        #include BOOST_PP_ITERATE()

    public:
        template<
            typename T
          , typename TCallStack
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        wrappers::universal<T> create_impl(TDeps& deps, TRefs& refs, const TVisitor& visitor, const TPolicies& policies) {
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
              , resolve<T, TCallStack>
            >(deps, refs, visitor, policies);
        }

    private:
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

    } // namespace core
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
        typedef data<T, TCallStack, TDependency> data_type;
        assert_policies<typename TPolicies::types, data_type>(policies);
        (visitor)(data_type());

        return binder_.template resolve_impl<
            T
          , typename ctor<TDependency>::type
          , TCallStack
          , TDependency
        >(*this, deps, refs, visitor, policies);
    }

#endif

