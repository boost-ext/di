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
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/providers/nothrow.hpp"
#include "boost/di/wrappers/universal.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/scopes/deduce.hpp"

#include <boost/units/detail/utility.hpp>
namespace boost {
namespace di {
namespace core {

BOOST_DI_HAS_TYPE(any);
BOOST_DI_HAS_TYPE(create2);
BOOST_DI_HAS_TYPE(create3);
BOOST_DI_HAS_TYPE(deps);
BOOST_DI_HAS_TYPE(given);
BOOST_DI_HAS_METHOD_CALL(configure, configure);
BOOST_DI_HAS_METHOD(call, call);

template<class T>
using is_injector = bool_<has_deps<T>::value || has_configure<T>::value>;

template<class T>
using is_dependency = has_given<T>;

template<class T, class = void>
struct get_deps {
    using type = typename T::deps;
};

template<class T>
struct get_deps<T, aux::enable_if_t<has_configure<T>{}>> {
    using type = typename aux::function_traits<decltype(&T::configure)>::result_type::deps;
};

template<class T, class = typename is_injector<T>::type, class = typename is_dependency<T>::type>
struct add_type_list;

template<class T, class TAny>
struct add_type_list<T, std::true_type, TAny> {
    using type = typename get_deps<T>::type;
};

template<class T>
struct add_type_list<T, std::false_type, std::true_type> {
    using type = type_list<T>;
};

template<class T>
struct add_type_list<T, std::false_type, std::false_type> {
    using type = type_list<dependency<scopes::deduce, T>>;
};

template<class... Ts>
using bindings_t = typename join<typename add_type_list<Ts>::type...>::type;

template<class TDeps = type_list<>>
class injector : public pool<TDeps> {
    template<class, class, class, class, class> friend class any_type;

    using pool_t = pool<TDeps>;

    template<class TDependency>
    using scope_create = typename aux::function_traits<decltype(&TDependency::create3)>::args;

    template<class T, class TDependency>
    struct data {
        using type = T;
        using dependency = TDependency;
        using binder = binder<TDeps>;
    };

    template<
        class T
      , class TDependency
      , class TProvider
      , class TRefs
      , class TVisitors
      , class... TArgs
    >
    struct provider;

    template<
        class T
      , class TDependency
      , class TProvider
      , class TRefs
      , class TVisitors
      , class... TArgs
    >
    class provider<T, TDependency, TProvider, TRefs, TVisitors, type_list<TArgs...>> {
    public:
        provider(injector& inj, const TProvider& provider, TRefs& refs, const TVisitors& visitors)
            : injector_(inj), provider_(provider), refs_(refs), visitors_(visitors)
        { }

        auto get() const {
            return provider_.template get<TDependency>(
                injector_.create<TArgs, T>(provider_, refs_, visitors_)...
            );
        }

    private:
        injector& injector_;
        const TProvider& provider_;
        TRefs& refs_;
        const TVisitors& visitors_;
    };

public:
    using deps = TDeps;

    // bind<...>, etc.  -> ignore
    // injector<...>    -> get all dependencies from the injector
    // dependency<...>  -> pass

    template<class... TArgs>
    injector(const TArgs&... args)
        : injector(init{}, pass_arg(args)...)
    { }

    template<class T>
    injector(injector<T> injector) // non explicit
        : pool_t(create(injector, TDeps{}), init{})
    { }

    template<class T, class... TArgs>
    T create(const TArgs&... args) {
        pool<type_list<TArgs...>> visitors(args...);
        std::vector<aux::shared_ptr<void>> refs_;
        return create<T, none_t>(providers::nothrow<pool_t>{static_cast<pool<deps>&>(*this)}, refs_, visitors);
    }

    template<class T, class TProvider, class... TArgs>
    T provide(const TProvider& provider, const TArgs&... args) {
        pool<type_list<TArgs...>> visitors(args...);
        std::vector<aux::shared_ptr<void>> refs_;
        return create<T, none_t>(provider, refs_, visitors);
    }

    template<class TAction>
    void call(const TAction& action) {
        call_impl(static_cast<pool<deps>&>(*this), action, deps{});
    }

private:
    template<class... TArgs>
    injector(const init&, const TArgs&... args)
        : pool_t(pool<type_list<TArgs...>>(args...), init{})
    { }

    template<
        class T
      , class TParent
      , class TProvider
      , class TRefs
      , class TVisitors
    >
    aux::enable_if_t<has_any<T>::value , any_type<TParent, injector, TProvider, TRefs, TVisitors> >
    create(const TProvider& provider, TRefs& refs, const TVisitors& visitors) {
        return any_type<TParent, injector, TProvider, TRefs, TVisitors>(
            *this, provider, refs, visitors
        );
    }

    template<
        class T
      , class TParent
      , class TProvider
      , class TRefs
      , class TVisitors
    >
    aux::enable_if_t<!has_any<T>::value, wrappers::universal<T>>
    create(const TProvider& provider, TRefs& refs, const TVisitors& visitors) {
        using dependency = typename binder<pool_t>::template resolve<T>;
        return create_impl<T, dependency>(provider, refs, visitors);
    }

    template<
        class T
      , class TDependency
      , class TProvider
      , class TRefs
      , class TVisitors
    >
    aux::enable_if_t<
        !size<scope_create<TDependency>>::value &&
        !has_create2<TDependency>::value
      , wrappers::universal<T>
    >
    create_impl(const TProvider&, TRefs& refs, const TVisitors&) {
        return {refs, acquire<TDependency>(static_cast<pool_t&>(*this)).create()};
    }

    template<
        class T
      , class TDependency
      , class TProvider
      , class TRefs
      , class TVisitors
    >
    aux::enable_if_t<
        size<scope_create<TDependency>>::value &&
        has_create2<TDependency>::value
      , wrappers::universal<T>
    >
    create_impl(const TProvider&, TRefs& refs, const TVisitors&) {
        return {refs, acquire<TDependency>(static_cast<pool_t&>(*this)).create_(*this)};
    }

    template<
        class T
      , class TDependency
      , class TProvider
      , class TRefs
      , class TVisitors
    >
    aux::enable_if_t<
        size<scope_create<TDependency>>::value &&
        !has_create2<TDependency>::value
      , wrappers::universal<T>
    >
    create_impl(const TProvider& provider_, TRefs& refs, const TVisitors& visitors) {
        using ctor = typename type_traits::ctor_traits<typename TDependency::given>::type;
        return { refs, acquire<TDependency>(static_cast<pool_t&>(*this)).create(
            provider<T, TDependency, TProvider, TRefs, TVisitors, ctor>{*this, provider_, refs, visitors}
        )};
    }


    template<class T, class... TVisitors>
    static void assert_visitors(const pool<type_list<TVisitors...>>& visitors) {
        int dummy[]{0, (assert_policy<TVisitors, T>(visitors), 0)...};
        (void)dummy;
    }

    template<class TPolicy, class T, class TVisitors>
    static void assert_policy(const TVisitors& visitors) {
        static_cast<const TPolicy&>(visitors).template assert_policy<T>();
    }

    template<class TDependency, class TDeps_>
    aux::enable_if_t<std::is_base_of<TDependency, TDeps_>{}, TDependency&>
    acquire(TDeps_& deps) {
        return static_cast<TDependency&>(deps);
    }

    template<class TDependency, class TDeps_>
    aux::enable_if_t<!std::is_base_of<TDependency, TDeps_>{}, TDependency>
    acquire(TDeps_&) {
        return TDependency{};
    }

    template<class TDeps_, class TAction, class... Ts>
    void call_impl(TDeps_& deps, const TAction& action, const type_list<Ts...>&) {
        int dummy[]{0, (call_impl<Ts>(deps, action), 0)...};
        (void)dummy;
    }

    template<class T, class TDeps_, class TAction>
    aux::enable_if_t<has_call<T, TAction>::value> call_impl(TDeps_& deps, const TAction& action) {
        static_cast<T&>(deps).call(action);
    }

    template<class T, class TDeps_, class TAction>
    aux::enable_if_t<!has_call<T, TAction>::value> call_impl(TDeps_&, const TAction&) { }

    template<class T>
    const auto& pass_arg(const T& arg, aux::enable_if_t<!has_configure<T>::value>* = 0) const {
        return arg;
    }

    template<class T>
    auto pass_arg(const T& arg, aux::enable_if_t<has_configure<T>{}>* = 0) const {
        return arg.configure();
    }

    template<typename T, typename... Ts>
    auto create(injector<T>& injector, const type_list<Ts...>&) {
        return pool<TDeps>(create_impl<Ts>(injector)...);
    }

    template<typename TDependency, typename T>
    auto create_impl(injector<T>& injector) {
        return TDependency{injector.template create<typename TDependency::given*>()};
    }
};

} // namespace core
} // namespace di
} // namespace boost

#endif

