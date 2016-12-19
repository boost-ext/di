###[Boost].DI design goals

* Be as fast as possible (See [Performance](overview.md#performance))
* Compile as fast as possible (See [Benchmarks](overview.md#benchmarks))
* Give short and intuitive error messages (See [Error messages](overview.md#error-messages))
* Guarantee object creation at compile-time (See [Create Objects Tree](tutorial.md#1-basic-create-objects-tree))
* Be as non-intrusive as possible (See [Injections](user_guide.md#injections))
* Be easy to extend (See [Extensions](extensions.md))

###Design Overview

[![Design](images/di.png)](images/di.png)

| Component  | Description |
| ---------- | ----------- |
| [Bindings] | DSL to create dependencies representation which will be used by core to resolve types |
| [Scopes]   | Responsible for maintain objects life time |
| [Providers]| Responsible for providing object instance |
| [Policies] | Compile-time limitations for types / Run-time types vistor |
| [Config]   | Configuration for [Policies] and [Providers] |
| Core       | Responsible for resolving requested types (implementation detail) |
| Wrappers   | Responsible for conversion to required type (implementation detail) |


<a id="nutshell"></a>

---

###In a nutshell (Implementation detail)

The main interface of the injector is a `create` method. 
When `create` method is called for type `T` the magic happens.
Firstly, policies are verified (for example, whether the type `T` is allowed to be created).
Then, the constructor traits are deduced (`ctor_traits` ) for type `T` and dependencies of the constructor
parameters are resolved (`binder`). `wrapper` is used to convert internal representation
of the dependency into a required type (ex. `shared_ptr<T>`). Whole process is repeated
recursively for all required parameters of `T` constructor.

```cpp
struct direct; // T(...)
struct uniform; // T{...}

template<class T, class... TArgs> is_braces_constructible; // type T is constructible using T{...}
template<class T, class... TArgs> is_constructible; // Type T is constructible using T(...)

template<class T> is_injectable; // Type T uses BOOST_DI_INJECT or BOOST_DI_INJECT_TRAITS

template<class TConfig, class... TBindings> // For Example, TBindings = { di::bind<Interface>.to<Implementation> }
struct core::injector : TBindings... {
  using config = TConfig;
  using deps = TBindings...;

  template<class T> // For example, T = Interface
  auto create() const noexcept {
    TConfig::policies<T>()...; // verify policies
    using Type = core::binder<TBindings...>().resolve<T>(*this); // Type = Implementation
    return core::wrapper<T>{dependency.create(provider<TInjector>{*this}.get<Type>())};
  }
};

template<class TInjector, class TConfig>
struct provider {
  template<class T> auto get() const noexcept {
    using pair<TInitialization, TCtor...> = decltype(ctor_traits<TInjector, T>());
    return TConfig::provider().get(TInitialization{}, TCtor...);
  }
  const TInjector& injector;
};

template<class TInjector>
struct any_type {
  template<class T>
  operator T() const {
    return injector.templte create<T>();
  }
  const TInjector& injector;
};

template<class TInjector, class T>
auto ctor_traits() {
  if (is_injectable<T>() {
    return pair<direct, typename T::boost_di_inject__>{}; // BOOST_DI_INJECT(T, args...) -> T(args...)
  }

  for (auto i = BOOST_DI_CFG_CTOR_LIMIT_SIZE; i >= 0; --i) {
    if (is_constructible<T, any_type<TInjector>...>()) { // ... -> i times
      return pair<direct, any_type<TInjector>...>{}; // T(any_type...)
    }
  }

  for (auto i = BOOST_DI_CFG_CTOR_LIMIT_SIZE; i >= 0; --i) {
    if (is_braces_constructible<T, any_type<TInjector>...>()) { // ... -> i times
      return pair<uniform, any_type<TInjector>...>{}; // T{any_type...}
    }
  }

  return error(...);
};
```

<span class="fa fa-eye wy-text-neutral warning"> **Note**<br/><br/>
[Automatic injection](user_guide.md#di_automatic) depends on template implicit conversion operator and therefore
conversion constructors `template<class I> T(I)` are not supported and have to be injected using [BOOST_DI_INJECT], [BOOST_DI_INJECT_TRAITS], [di::inject] or [di::ctor_traits].
</span>

[Bindings]: user_guide.md#bindings
[Scopes]: user_guide.md#scopes
[Providers]: user_guide.md#providers
[Policies]: user_guide.md#policies
[Config]: user_guide.md#di_config
[di::inject]: #di_inject
[di::ctor_traits]: #di_ctor_traits
[BOOST_DI_INJECT]: #BOOST_DI_INJECT
[BOOST_DI_INJECT_TRAITS]: #BOOST_DI_INJECT_TRAITS
