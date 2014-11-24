//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_INJECTOR_HPP
#define BOOST_DI_CORE_INJECTOR_HPP

#include <vector>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/core/creator.hpp"
#include "boost/di/providers/nothrow.hpp"

namespace boost {
namespace di {
namespace core {

template<typename TDeps = type_list<>, typename TDefaultProvider = providers::nothrow>
class injector : public pool<TDeps> {
    using pool_t = pool<TDeps>;

    struct empty_visitor {
        template<typename T>
        void operator()(const T&) const { }
    };

public:
    using deps = TDeps;

    // bind<...>, etc.  -> ignore
    // injector<...>    -> get all dependencies from the injector
    // dependency<...>  -> pass

    template<typename... TArgs>
    injector(const TArgs&... args)
        : pool_t(pool<type_list<TArgs...>>(args...), init{})
        , creator_{*this}
    { }

    template<typename T, typename... TArgs>
    T create(const TArgs&... args) {
        //TODO check if there is a provider or visitor within args?
        pool<type_list<TArgs...>> policies_(args...);
        std::vector<aux::shared_ptr<void>> refs_;

        return creator_.template create<T, none_t>(
            TDefaultProvider{}
          , static_cast<pool_t&>(*this)
          , refs_
          , empty_visitor{}
          , policies_
        );
    }

    template<typename TAction>
    void call(const TAction& action) {
        call_impl(static_cast<pool<deps>&>(*this), action, deps{});
    }

private:
    template<typename TDeps_, typename TAction, typename... Ts>
    void call_impl(TDeps_& deps, const TAction& action, const type_list<Ts...>&) {
        int dummy[]{0, (call_impl<Ts>(deps, action), 0)...};
        (void)dummy;
    }

    template<typename T, typename TDeps_, typename TAction>
    aux::enable_if_t<aux::has_call<T>{}> call_impl(TDeps_& deps, const TAction& action) {
        static_cast<T&>(deps).call(action);
    }

    template<typename T, typename TDeps_, typename TAction>
    aux::enable_if_t<!aux::has_call<T>{}> call_impl(TDeps_&, const TAction&) { }

    creator<injector> creator_;
};

} // namespace core
} // namespace di
} // namespace boost

#endif

