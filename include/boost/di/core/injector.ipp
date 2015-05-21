BOOST_DI_CORE_INJECTOR {
    template<class> friend class scopes::exposed;
    template<class, class, class> friend struct any_type;
    template<class, class> friend struct successful::any_type;
    template<class, class, class> friend struct any_type_ref;
    template<class, class> friend struct successful::any_type_ref;
    template<class, class, class, class> friend struct try_provider;
    template<class, class, class, class, class> friend struct provider;
    template<class, class, class, class> friend struct successful::provider;
    template<class, class, class> friend struct is_creatable_impl;

    using pool_t = pool<transform_t<TDeps...>>;
    using is_root_t = std::true_type;
    using config_t = type_traits::config_traits_t<TConfig, injector>;
    using config = std::conditional_t<
        std::is_default_constructible<TConfig>::value
      , _
      , config_t
    >;

    template<
        class T
      , class TName = no_name
      , class TIsRoot = std::false_type
      , class TDependency = std::remove_reference_t<decltype(binder::resolve<T, TName>((injector*)0))>
      , class TCtor = typename type_traits::ctor_traits<typename TDependency::given>::type
    > static auto try_create_impl() -> std::enable_if_t<std::is_convertible<
       decltype(
           std::declval<TDependency>().template try_create<T>(
               try_provider<
                   typename TDependency::given
                 , TCtor
                 , injector
                 , decltype(((TConfig*)0)->provider())
               >{}
           )
       ), T>::value
       BOOST_DI_TRY_POLICY
    >;

    static auto is_creatable_impl(...) -> std::false_type;

    template<class T, class TName, class TIsRoot>
    static auto is_creatable_impl(T&&, TName&&, TIsRoot&&)
        -> aux::is_valid_expr<decltype(try_create_impl<T, TName, TIsRoot>())>;

    template<class T, class TName = no_name, class TIsRoot = std::false_type>
    #if defined(BOOST_DI_MSVC)
        struct is_creatable : std::true_type { };
    #else
        using is_creatable =
            decltype(is_creatable_impl(std::declval<T>(), std::declval<TName>(), std::declval<TIsRoot>()));
    #endif

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
    struct try_create<type_traits::named<TName, T>> {
        using type = std::conditional_t<is_creatable<T, TName>::value, T, void>;
    };

public:
    using deps = transform_t<TDeps...>;

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
        return create_successful_impl<is_root_t>(aux::type<T>{});
    }

    template<class T, BOOST_DI_REQUIRES(!is_creatable<T, no_name, is_root_t>::value)>
    BOOST_DI_CONCEPTS_CREATABLE_ATTR
    T create() const {
        return create_impl<is_root_t>(aux::type<T>{});
    }

    template<class TAction>
    void call(const TAction& action) {
        call_impl(action, deps{});
    }

private:
    template<class... TArgs>
    explicit injector(const from_deps&, const TArgs&... args) noexcept
        : pool_t{copyable<deps>{}, pool<aux::type_list<TArgs...>>{args...}}
        , config{*this}
    { }

    template<class TInjector, class... TArgs>
    explicit injector(const from_injector&, const TInjector& injector, const aux::type_list<TArgs...>&) noexcept
        : pool_t{copyable<deps>{}, pool_t{build<TArgs>(injector)...}}
        , config{*this}
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
    auto create_impl(const aux::type<type_traits::named<TName, T>>&) const {
        return create_impl__<TIsRoot, T, TName>();
    }

    template<class TIsRoot = std::false_type, class T, class TName = no_name>
    auto create_impl__() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t>::type;
        using provider_t = core::provider<expected_t, given_t, TName, ctor_t, injector>;
        using wrapper_t = decltype(dependency.template create<T>(provider_t{*this}));
        using create_t = type_traits::referable_traits_t<T, dependency_t>;
        BOOST_DI_APPLY_POLICY
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
    auto create_successful_impl(const aux::type<type_traits::named<TName, T>>&) const {
        return create_successful_impl__<TIsRoot, T, TName>();
    }

    template<class TIsRoot = std::false_type, class T, class TName = no_name>
    auto create_successful_impl__() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t>::type;
        using provider_t = successful::provider<expected_t, given_t, ctor_t, injector>;
        using wrapper_t = decltype(dependency.template create<T>(provider_t{*this}));
        using create_t = type_traits::referable_traits_t<T, dependency_t>;
        BOOST_DI_APPLY_POLICY
        return successful::wrapper<create_t, wrapper_t>{dependency.template create<T>(provider_t{*this})};
    }

    template<class TAction, class... Ts>
    void call_impl(const TAction& action, const aux::type_list<Ts...>&) {
        int _[]{0, (call_impl<Ts>(action, has_call<Ts, const TAction&>{}), 0)...}; (void)_;
    }

    template<class T, class TAction>
    void call_impl(const TAction& action, const std::true_type&) {
        static_cast<T&>(*this).call(action);
    }

    template<class, class TAction>
    void call_impl(const TAction&, const std::false_type&) { }
};

