//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_MODULE_HPP
#define BOOST_DI_CORE_MODULE_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/core/creator.hpp"
#include "boost/di/core/allocator.hpp"
#include "boost/di/type_traits/has_call.hpp"

#include <vector>

namespace boost {
namespace di {
namespace core {

template<typename TDeps>
class module : public pool<TDeps> {
    template<typename> friend class module;
    using pool_t = pool<TDeps>;

    struct has_bindings {
        template<typename T>
        struct apply
            : bool_<!(has_types<T>::value || contains<TDeps, T>::value)>
        { };
    };

    class empty_visitor {
    public:
        template<typename T>
        void operator()(const T&) const { }
    };

public:
    using deps = TDeps;

    // bind<...>, etc.   -> ignore
    // module<....>      -> get all dependencies from the module
    // dependency<....>  -> pass

    template<typename... TArgs>
    explicit module(const TArgs&... args)
        : pool_t(pool<type_list<TArgs...>, has_bindings>(args...), init())
    { }

    template<typename T, typename... TPolicies>
    T create(const TPolicies&... policies) {
        using call_stack = type_list<>;
        pool<type_list<TPolicies...>> policies_(policies...);
        std::vector<aux::shared_ptr<void> > refs_;

        return creator_.template create<T, T, call_stack>(
            allocator()
          , static_cast<pool_t&>(*this)
          , refs_
          , empty_visitor()
          , policies_
        );
    }

    template<typename T, typename TAllocator, typename... TPolicies>
    T allocate(const TAllocator& allocator, const TPolicies&... policies) {
        using call_stack = type_list<>;
        pool<type_list<TPolicies...>> policies_(policies...);
        std::vector<aux::shared_ptr<void> > refs_;

        return creator_.template create<T, T, call_stack>(
            allocator
          , static_cast<pool_t&>(*this)
          , refs_
          , empty_visitor()
          , policies_
        );
    }

    template<typename T, typename TVisitor>
    T visit(const TVisitor& visitor) {
        using call_stack = type_list<>;
        std::vector<aux::shared_ptr<void> > refs_;

        return creator_.template create<T, T, call_stack>(
            allocator()
          , static_cast<pool_t&>(*this)
          , refs_
          , visitor
          , pool<>()
        );
    }

    template<typename TAction>
    void call(const TAction& action) {
        //call_impl<deps>(static_cast<pool<deps>&>(*this), action);
    }

private:
/*        template<typename TSeq, typename T, typename TAction>*/
    //typename std::enable_if<aux::mpl::empty<TSeq> >::type call_impl(T&, const TAction&) { }

    //template<typename TSeq, typename T, typename TAction>
    //typename std::enable_if<
        //aux::mpl::and_<
            //aux::mpl::not_<aux::mpl::empty<TSeq> >
          //, type_traits::has_call<typename aux::mpl::front<TSeq>::type, TAction>
        //>
    //>::type
    //call_impl(T& deps, const TAction& action) {
        //static_cast<typename aux::mpl::front<TSeq>::type&>(deps).call(action);
        //call_impl<typename aux::mpl::pop_front<TSeq>::type>(deps, action);
    //}

    //template<typename TSeq, typename T, typename TAction>
    //typename disable_if<
        //aux::mpl::or_<
            //aux::mpl::empty<TSeq>
          //, type_traits::has_call<typename aux::mpl::front<TSeq>::type, TAction>
        //>
    //>::type
    //call_impl(T& deps, const TAction& action) {
        //call_impl<typename aux::mpl::pop_front<TSeq>::type>(deps, action);
    /*}*/

    creator<TDeps> creator_;
};

} // namespace core
} // namespace di
} // namespace boost

#endif

