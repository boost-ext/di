//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
template<class TConfig BOOST_DI_CORE_INJECTOR_POLICY(, class TPolicies = pool<>)(), class... TDeps>
class injector BOOST_DI_CORE_INJECTOR_POLICY()(<TConfig, pool<>, TDeps...>)
    : pool<transform_t<TDeps...>> {
    friend class binder;
    template<class> friend class pool;
    template<class> friend class scopes::exposed;
    template<class, class, class> friend struct any_type;
    template<class, class> friend struct successful::any_type;
    template<class, class, class> friend struct any_type_ref;
    template<class, class> friend struct successful::any_type_ref;
    template<class, class, class, class> friend struct try_provider;
    template<class, class, class, class, class> friend struct provider;
    template<class, class, class, class> friend struct successful::provider;

    using pool_t = pool<transform_t<TDeps...>>;
    using is_root_t = std::true_type;

public:
    using deps = transform_t<TDeps...>;
    using config = TConfig;

    template<class T, class TName = no_name, class TIsRoot = std::false_type>
    struct is_creatable {
        using TDependency = std::remove_reference_t<decltype(binder::resolve<T, TName>((injector*)0))>;
        using TCtor = typename type_traits::ctor_traits__<typename TDependency::given>::type;

        static constexpr auto value = std::is_convertible<
            decltype(
                TDependency::template try_create<T>(
                    try_provider<
                        typename TDependency::given
                      , TCtor
                      , injector
                      , decltype(TConfig::provider(std::declval<injector>()))
                    >{}
                )
            ), T>::value BOOST_DI_CORE_INJECTOR_POLICY(&&
            policy::template try_call<
                arg_wrapper<type_traits::referable_traits_t<T, TDependency>, TName, TIsRoot, pool_t>
              , TPolicies
              , TDependency
              , TCtor
            >::value)();
    };

    template<class... TArgs>
    explicit injector(const init&, const TArgs&... args) noexcept
        : injector{from_deps{}, get_arg(args, has_configure<decltype(args)>{})...}
    { }

    template<class TConfig_, class TPolicies_, class... TDeps_>
    explicit injector(const injector<TConfig_, TPolicies_, TDeps_...>& other) noexcept
        : injector{from_injector{}, other, deps{}}
    { }

    template<class T, BOOST_DI_REQUIRES(is_creatable<T, no_name, is_root_t>::value)>
    T create() const {
        return BOOST_DI_TYPE_WKND(T)create_successful_impl<is_root_t>(aux::type<T>{});
    }

    template<class T, BOOST_DI_REQUIRES(!is_creatable<T, no_name, is_root_t>::value)>
    BOOST_DI_CONCEPTS_CREATABLE_ATTR
    T create() const {
        return BOOST_DI_TYPE_WKND(T)create_impl<is_root_t>(aux::type<T>{});
    }

    template<class T, BOOST_DI_REQUIRES(!has_deps<T>::value)>
    operator T() const {
        return create<T>();
    }

private:
    template<class T>
    struct try_create {
        using type = std::conditional_t<is_creatable<T>::value, T, void>;
    };

    template<class TParent>
    struct try_create<any_type_fwd<TParent>> {
        using type = any_type<TParent, injector, with_error>;
    };

    template<class TParent>
    struct try_create<any_type_ref_fwd<TParent>> {
        using type = any_type_ref<TParent, injector, with_error>;
    };

    template<class TName, class T>
    struct try_create<detail::named_type<TName, T>> {
        using type = std::conditional_t<is_creatable<T, TName>::value, T, void>;
    };

    template<class... TArgs>
    explicit injector(const from_deps&, const TArgs&... args) noexcept
        : pool_t{copyable_t<deps>{}, core::pool_t<TArgs...>{args...}}
    { }

    template<class TInjector, class... TArgs>
    explicit injector(const from_injector&, const TInjector& injector, const aux::type_list<TArgs...>&) noexcept
    #if defined(BOOST_DI_MSVC)
        : pool_t{copyable_t<deps>{}, pool_t{build<TArgs>(injector)...}}
    #else
        : pool_t{copyable_t<deps>{}, pool_t{TArgs{injector}...}}
    #endif
    { }

    template<class TIsRoot = std::false_type, class T>
    auto create_impl(const aux::type<T>&) const {
        return create_impl__<TIsRoot, T>();
    }

    template<class TIsRoot = std::false_type, class TParent>
    auto create_impl(const aux::type<any_type_fwd<TParent>>&) const {
        return any_type<TParent, injector>{*this};
    }

    template<class TIsRoot = std::false_type, class TParent>
    auto create_impl(const aux::type<any_type_ref_fwd<TParent>>&) const {
        return any_type_ref<TParent, injector>{*this};
    }

    template<class TIsRoot = std::false_type, class T, class TName>
    auto create_impl(const aux::type<detail::named_type<TName, T>>&) const {
        return create_impl__<TIsRoot, T, TName>();
    }

    template<class TIsRoot = std::false_type, class T, class TName = no_name>
    auto create_impl__() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using provider_t = core::provider<expected_t, given_t, TName, ctor_t, injector>;
        using wrapper_t = decltype(dependency.template create<T>(provider_t{*this}));
        using create_t = type_traits::referable_traits_t<T, dependency_t>;
        BOOST_DI_CORE_INJECTOR_POLICY(
            policy::template call<arg_wrapper<create_t, TName, TIsRoot, pool_t, std::true_type>>(
                TConfig::policies(*this), dependency, ctor_t{}
            );
        )()
        return wrapper<create_t, wrapper_t>{dependency.template create<T>(provider_t{*this})};
    }

    template<class TIsRoot = std::false_type, class T>
    auto create_successful_impl(const aux::type<T>&) const {
        return create_successful_impl__<TIsRoot, T>();
    }

    template<class TIsRoot = std::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_fwd<TParent>>&) const {
        return successful::any_type<TParent, injector>{*this};
    }

    template<class TIsRoot = std::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_ref_fwd<TParent>>&) const {
        return successful::any_type_ref<TParent, injector>{*this};
    }

    template<class TIsRoot = std::false_type, class T, class TName>
    auto create_successful_impl(const aux::type<detail::named_type<TName, T>>&) const {
        return create_successful_impl__<TIsRoot, T, TName>();
    }

    template<class TIsRoot = std::false_type, class T, class TName = no_name>
    auto create_successful_impl__() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using provider_t = successful::provider<expected_t, given_t, ctor_t, injector>;
        using wrapper_t = decltype(dependency.template create<T>(provider_t{*this}));
        using create_t = type_traits::referable_traits_t<T, dependency_t>;
        BOOST_DI_CORE_INJECTOR_POLICY(
            policy::template call<arg_wrapper<create_t, TName, TIsRoot, pool_t, std::true_type>>(
                TConfig::policies(*this), dependency, ctor_t{}
            );
        )()
        return successful::wrapper<create_t, wrapper_t>{dependency.template create<T>(provider_t{*this})};
    }
};

