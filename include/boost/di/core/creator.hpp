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
    #include "boost/di/core/binder.hpp"
    #include "boost/di/core/any_type.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"
    #include "boost/di/type_traits/function_traits.hpp"
    #include "boost/di/wrappers/universal.hpp"

    #include <typeinfo>
    #include <map>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/ref.hpp>
    #include <boost/none_t.hpp>
    #include <boost/bind.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/x11/vector.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/x11/size.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/push_back.hpp>

    namespace boost {
    namespace di {
    namespace core {

    template<
        typename TDependecies = mpl::vector0<>
      , template<typename> class TBinder = binder
      , template<
            typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
        >
        class TAnyType = ::boost::di::core::any_type
    >
    class creator
    {
        template<typename TDependency>
        struct scope_create
            : type_traits::function_traits<
                  BOOST_DI_FEATURE_DECLTYPE(&TDependency::create)
              >::type
        { };

        class type_comparator
        {
        public:
            bool operator()(const std::type_info* lhs, const std::type_info* rhs) const {
                return lhs->before(*rhs);
            }
        };

        typedef std::map<
            const std::type_info*
          , aux::shared_ptr<void>
          , type_comparator
        > scopes_type;

        template<
            typename T
          , typename TCallStack
          , typename TDependency
        >
        struct data
        {
            typedef T type;
            typedef TCallStack call_stack;
            typedef TDependency dependency;
            typedef TBinder<TDependecies> binder;
        };

    public:
        explicit creator(const TBinder<TDependecies>& binder = TBinder<TDependecies>())
            : binder_(binder)
        { }

        template<
            typename T
          , typename TParent // ignore copy/move ctor
          , typename TCallStack
          , typename TAllocator
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        typename enable_if<
            is_same<T, TAnyType<> >
          , TAnyType<
                TParent
              , TCallStack
              , creator
              , TAllocator
              , TDeps
              , TRefs
              , TVisitor
              , TPolicies
            >
        >::type
        create(const TAllocator& allocator
             , TDeps& deps
             , TRefs& refs
             , const TVisitor& visitor
             , const TPolicies& policies) {
            return TAnyType<
                TParent
              , TCallStack
              , creator
              , TAllocator
              , TDeps
              , TRefs
              , TVisitor
              , TPolicies
            >(*this, allocator, deps, refs, visitor, policies);
        }

        template<
            typename T
          , typename // TParent - not used
          , typename TCallStack
          , typename TAllocator
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        typename disable_if<is_same<T, TAnyType<> >, wrappers::universal<T> >::type
        create(const TAllocator& allocator
             , TDeps& deps
             , TRefs& refs
             , const TVisitor& visitor
             , const TPolicies& policies) {
            typedef typename TBinder<TDependecies>::template
                resolve<T, TCallStack>::type dependency_type;

            BOOST_DI_FEATURE_EXAMINE_CALL_STACK(
                typedef typename mpl::push_back<TCallStack, T>::type call_stack_type;
            )

            BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(
                typedef TCallStack call_stack_type;
            )

            typedef data<T, call_stack_type, dependency_type> data_type;
            assert_policies<typename TPolicies::types, data_type>(policies);
            (visitor)(data_type());

            return create_impl<T, dependency_type, call_stack_type>(
                allocator, deps, refs, visitor, policies
            );
        }

    private:
        #define BOOST_PP_FILENAME_1 "boost/di/core/creator.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM_FORCE(0)
        #include BOOST_PP_ITERATE()

        template<
            typename T
          , typename TDependency
          , typename TCallStack
          , typename TAllocator
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        typename enable_if_c<
            !mpl::x11::size<scope_create<TDependency> >::value
          , wrappers::universal<T>
        >::type
        create_impl(const TAllocator&
                  , TDeps& deps
                  , TRefs& refs
                  , const TVisitor&
                  , const TPolicies&) {
            return wrappers::universal<T>(
                refs, acquire<TDependency>(deps).create()
            );
        }

        template < class T, class R >
        struct normalize;

        template < class... TTypes, class X >
        struct normalize< mpl::x11::vector< TTypes... >, X >
           : mpl::x11::vector< TTypes..., X >
        { };

        template<
            typename T
          , typename TDependency
          , typename TCallStack
          , typename TAllocator
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        typename disable_if_c<
            !mpl::x11::size<scope_create<TDependency> >::value
          , wrappers::universal<T> >::type
        create_impl(const TAllocator& allocator
                  , TDeps& deps
                  , TRefs& refs
                  , const TVisitor& visitor
                  , const TPolicies& policies) {
            typedef typename mpl::x11::fold<
                type_traits::ctor_traits<typename TDependency::given>
              , mpl::x11::vector<>
              , normalize<mpl::x11::arg<0>, mpl::x11::arg<1> >
            >::type ctor_type;

            return wrappers::universal<T>(
                refs
              , acquire<TDependency>(deps).create(
                    boost::bind(
                        static_cast<
                            typename TDependency::expected*(creator::*)(
                                const TAllocator&
                              , TDeps&
                              , TRefs&
                              , const TVisitor&
                              , const TPolicies&
                              , const ctor_type&
                            )
                        >(&creator::create_impl<T, TDependency, TCallStack>)
                      , this
                      , allocator
                      , deps
                      , refs
                      , visitor
                      , policies
                      , ctor_type()
                    )
                )
            );
        }

        template<typename TSeq, typename, typename TPolicies>
        static typename enable_if<mpl::empty<TSeq> >::type
        assert_policies(const TPolicies&) { }

        template<typename TSeq, typename T, typename TPolicies>
        static typename disable_if<mpl::empty<TSeq> >::type
        assert_policies(const TPolicies& policies) {
            typedef typename mpl::front<TSeq>::type policy;
            static_cast<const policy&>(policies).template assert_policy<T>();
            assert_policies<typename mpl::pop_front<TSeq>::type, T>(policies);
        }

        template<typename TDependency, typename TDeps>
        typename enable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
        acquire(TDeps& deps) {
            return static_cast<TDependency&>(deps);
        }

        template<typename TDependency, typename TDeps>
        typename disable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
        acquire(TDeps&) {
            typename scopes_type::const_iterator it = scopes_.find(&typeid(TDependency));
            if (it != scopes_.end()) {
                return *static_cast<TDependency*>(it->second.get());
            }

            aux::shared_ptr<TDependency> dependency(new TDependency());
            scopes_[&typeid(TDependency)] = dependency;
            return *dependency;
        }

        TBinder<TDependecies> binder_;
        scopes_type scopes_;
    };

    } // namespace core
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TAllocator
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
      , BOOST_DI_TYPES(TArgs)
    >
    typename TDependency::expected*
    create_impl(const TAllocator& allocator
              , TDeps& deps
              , TRefs& refs
              , const TVisitor& visitor
              , const TPolicies& policies
              , const mpl::x11::vector<BOOST_DI_TYPES_PASS(TArgs)>&) {
        return allocator.template
            allocate<typename TDependency::expected, typename TDependency::given>(
                create<TArgs, T, TCallStack>(allocator, deps, refs, visitor, policies)...
        );
    }

#endif

