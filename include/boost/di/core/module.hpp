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

#include <vector>

namespace boost {
namespace di {
namespace core {

BOOST_DI_HAS_MEMBER_FUNCTION(call, call);

template<typename TDeps = type_list<>>
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
        std::vector<aux::shared_ptr<void>> refs_;

        return creator_.template create<T, none_t, call_stack>(
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
        std::vector<aux::shared_ptr<void>> refs_;

        return creator_.template create<T, none_t, call_stack>(
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
        std::vector<aux::shared_ptr<void>> refs_;

        return creator_.template create<T, none_t, call_stack>(
            allocator()
          , static_cast<pool_t&>(*this)
          , refs_
          , visitor
          , pool<>()
        );
    }

    template<typename TAction>
    void call(const TAction& action) {
        call_impl(static_cast<pool<deps>&>(*this), action, deps());
    }

private:
    template<typename TDeps_, typename TAction, typename... Ts>
    void call_impl(TDeps_& deps, const TAction& action, const type_list<Ts...>&) {
         int dummy[]{0, (call_impl<Ts>(deps, action), 0)...};
         (void)dummy;
    }

    template<typename T, typename TDeps_, typename TAction>
    typename std::enable_if<has_call<T>::value>::type
    call_impl(TDeps_& deps, const TAction& action) {
        static_cast<T&>(deps).call(action);
    }

    template<typename T, typename TDeps_, typename TAction>
    typename std::enable_if<!has_call<T>::value>::type
    call_impl(TDeps_& deps, const TAction& action) { }

    creator<TDeps> creator_;
};

} // namespace core
} // namespace di
} // namespace boost

#endif

