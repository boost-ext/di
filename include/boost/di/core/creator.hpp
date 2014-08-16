//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_CREATOR_HPP
#define BOOST_DI_CORE_CREATOR_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/function_traits.hpp"
#include "boost/di/wrappers/universal.hpp"

#include <typeinfo>
#include <map>

namespace boost {
namespace di {
namespace core {

BOOST_DI_HAS_MEMBER_TYPE(any);

template<typename TDeps_>
class creator {
    using binder_t = binder<TDeps_>;

    template<typename TDependency>
    using scope_create = typename type_traits::function_traits<
          decltype(&TDependency::create)
      >::type;

    class type_comparator {
    public:
        bool operator()(const std::type_info* lhs, const std::type_info* rhs) const {
            return lhs->before(*rhs);
        }
    };

    using scopes_type = std::map<
        const std::type_info*
      , aux::shared_ptr<void>
      , type_comparator
    >;

    template<
        typename T
      , typename TCallStack
      , typename TDependency
    >
    struct data {
        using type = T;
        using call_stack = TCallStack;
        using dependency = TDependency;
        using binder = binder_t;
    };

public:
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
    typename std::enable_if<
        has_any<T>::value
      , any_type<
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
        return any_type<
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
    typename std::enable_if<!has_any<T>::value, wrappers::universal<T>>::type
    create(const TAllocator& allocator
         , TDeps& deps
         , TRefs& refs
         , const TVisitor& visitor
         , const TPolicies& policies) {
        using dependency_type = typename binder_t::template
            resolve<T, TCallStack>::type;

        //typedef data<T, call_stack_type, dependency_type> data_type;
        //assert_policies<typename TPolicies::types, data_type>(policies);
        //(visitor)(data_type());

        return create_impl<T, dependency_type, TCallStack>(
            allocator, deps, refs, visitor, policies
        );
    }

private:
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
    typename std::enable_if<
        !size<typename scope_create<TDependency>::type>::value
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
    typename std::enable_if<
        size<typename scope_create<TDependency>::type>::value
      , wrappers::universal<T>>::type
    create_impl(const TAllocator& allocator
              , TDeps& deps
              , TRefs& refs
              , const TVisitor& visitor
              , const TPolicies& policies) {

        return wrappers::universal<T>(
            refs
          , acquire<TDependency>(deps).create(
              [&] {
                using ctor_type = typename type_traits::ctor_traits<typename TDependency::given>::type;

                //BOOST_DI_FEATURE_EXAMINE_CALL_STACK(
                    //using call_stack_type = typename add<TCallStack, T>::type;
                //)

                //BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(
                    using call_stack_type = TCallStack;
                //)

                return create_impl<T, TDependency, call_stack_type>(allocator, deps, refs, visitor, policies, ctor_type());}
            )
        );
    }

    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TAllocator
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
      , typename... TArgs
    >
    typename TDependency::expected*
    create_impl(const TAllocator& allocator
              , TDeps& deps
              , TRefs& refs
              , const TVisitor& visitor
              , const TPolicies& policies
              , const type_list<TArgs...>&) {
        return allocator.template
            allocate<typename TDependency::expected, typename TDependency::given>(
                create<TArgs, T, TCallStack>(allocator, deps, refs, visitor, policies)...
        );
    }

/*        template<typename TSeq, typename, typename TPolicies>*/
    //static typename enable_if<aux::mpl::empty<TSeq>>::type
    //assert_policies(const TPolicies&) { }

    //template<typename TSeq, typename T, typename TPolicies>
    //static typename disable_if<aux::mpl::empty<TSeq>>::type
    //assert_policies(const TPolicies& policies) {
        //typedef typename aux::mpl::front<TSeq>::type policy;
        //static_cast<const policy&>(policies).template assert_policy<T>();
        //assert_policies<typename aux::mpl::pop_front<TSeq>::type, T>(policies);
    /*}*/

    template<typename TDependency, typename TDeps>
    typename std::enable_if<std::is_base_of<TDependency, TDeps>::value, TDependency&>::type
    acquire(TDeps& deps) {
        return static_cast<TDependency&>(deps);
    }

    template<typename TDependency, typename TDeps>
    typename std::enable_if<!std::is_base_of<TDependency, TDeps>::value, TDependency&>::type
    acquire(TDeps&) {
        typename scopes_type::const_iterator it = scopes_.find(&typeid(TDependency));
        if (it != scopes_.end()) {
            return *static_cast<TDependency*>(it->second.get());
        }

        aux::shared_ptr<TDependency> dependency(new TDependency());
        scopes_[&typeid(TDependency)] = dependency;
        return *dependency;
    }

    binder_t binder_;
    scopes_type scopes_;
};

} // namespace core
} // namespace di
} // namespace boost

#endif

