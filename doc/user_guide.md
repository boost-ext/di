* [Injector](#injector)
    * [di::injector]()
    * [di::make_injector]()
* [Bindings](#bindings)
    * [di::bind]()
* [Injections](#injections)
    * [automatic (default)]()
    * [BOOST_DI_INJECT]()
    * [di::ctor_traits]()
* [Annotations](#annotations)
    * [(named = name)]()
* [Scopes](#scopes)
    * [di::deduce (default)]()
    * [di::instance (di::bind<>.to(value))]()
    * [di::singleton]()
    * [di::unique]()
* [Modules](#modules)
* [Providers](#providers)
    * [di::providers::stack_over_heap (default)]()
    * [di::providers::heap]()
* [Policies](#policies)
    * [di::policies::constructible]()
* [Concepts](#concepts)
    * [di::concepts::boundable]()
    * [di::concepts::callable]()
    * [di::concepts::configurable]()
    * [di::concepts::creatable]()
    * [di::concepts::providable]()
    * [di::concepts::scopable]()
* [Configuration](#configuration)

---

###Injector

```cpp
di::injector
```

***Header***

    #include <boost/di.hpp>

***Description***

Injector is a component used for creating dependencies configuration using bindings and resolve requested types. Boost.DI injector might be composed by mixing bindings, modules and other injectors.

***Semantics***

    template<class... TDeps> requires boundable<TDeps...>
    class injector {
    public:
      using deps; // list of dependencies
      using config; // configuration

      injector(injector&&) = default;
      template <class... Ts> // no requirements
      injector(core::injector<Ts...>&&) noexcept;
      explicit injector(const TDeps&...) noexcept;

      template<class T> requires creatable<T>
      T create() const;

      template <class T> requires creatable<T>
      operator T() const;
    };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `TDeps...` | [boundable]<TDeps...\> | [Bindings] to be used as configuration | - |
| `create<T>()` | [creatable]<T\> | Creates type `T` | `T` |
| `operator T()` | [creatable]<T\> | Creates type `T` | `T` |

| Type `T` | Is allowed? | Note |
| -------- | ----------- | ---- |
| `T` | ✔ | - |
| `T*` | ✔ | Ownerhsip transfer! |
| `const T*` | ✔ | Ownerhsip transfer! |
| `T&` | ✔ | - |
| `const T&` | ✔ | - |
| `T&&` | ✔ | - |
| `std::unique_ptr<T>` | ✔ | - |
| `std::shared_ptr<T>` | ✔ | - |
| `std::weak_ptr<T>` | ✔ | - |
| `boost_shared_ptr<T>` | ✔ | - |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::make_injector
```

***Header***

    #include <boost/di.hpp>

***Description***

Creates [injector] type.

***Semantics***

    template<
      class TConfig = di::config
    , class... TBindings
    > requires boundable<TBindings...> && configurable<TConfig>
    auto make_injector(const TBindings&...) noexcept;

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `TConfig` | [configurable]<TConfig\> | [Configuration] | - |
| `make_injector(const TBindings&...)` | [boundable]<TBindings...\> | Creates [injector] with given [Bindings] | [injector] |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

###Bindings

Bindings define dependencies configuration which basically means what types will be created
and what values will be passed into them.

```cpp
di::bind
```

***Header***

    #include <boost/di.hpp>

***Description***

Allows to bind interface to implementation and associate value with it.

***Semantics***

    struct override; // overrides given configuration
    
    namespace detail {
      template<class... Ts> requires boundable<Ts...>
      struct bind {
        bind(bind&&) noexcept = default;
    
        template<class T> requires boundable<T>
        auto to() noexcept;
    
        template<class T> requires boundable<T>
        auto to(T&&) noexcept;
    
        template<class TScope> requires scopable<TScope>
        auto in(const TScope& = di::deduce) noexcept;
    
        template<class TName> // no requirements
        auto named(const TName& = {}) noexcept;
    
        auto operator[](const override&) noexcept;
      };
    } // detail
    
    template<class... Ts> requires boundable<Ts...>
    detail::bind<Ts...> bind{};

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `Ts...` | [boundable]<Ts...\> | 'Interface' types | - |
| `to<T\>` | [boundable]<T\> | Binds `Ts...` to `T` type | [boundable] |
| `to(T&&)` | [boundable]<T\> | Binds `Ts...` to `T` object | [boundable] |
| `in(const TScope&)` | [scopable]<TScope\> | Binds `Ts...` into `TScope` | [boundable] |
| `named(const TName&)` | - | Binds `Ts...` using 'Named' annotation | [boundable] |
| `operator[](const override&)` | - | Overrides given binding | [boundable] |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_interface_to_implementation.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_type_to_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_deduce_type_to_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_type_to_compile_time_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_multiple_interfaces.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_cross_platform.cpp)
***Example***

![CPP(BTN)](Run_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/bindings.cpp)
![CPP(BTN)](Run_Dynamic_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/dynamic_bindings.cpp)
![CPP(BTN)](Run_Multiple_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/multiple_bindings.cpp)

&nbsp;

---

###Injections

*Constructor Injection* is the most powerful of available injections.
It guarantees initialized state of data members. Boost.DI constructor injection is achieved without any additional work from the user.

```cpp
automatic (default)
```

***Header***

    #include <boost/di.hpp>

***Description***

Boost.DI will deduce the best available constructor to be used for injection - unique constructor with the longest parameter list.
If the default behavior should be changed constructor has to be explicitly marked with [BOOST_DI_INJECT] or [BOOST_DI_INJECT_TRAITS].

***Semantics***

    class constructor {
    public:
      constructor(auto parameter1, auto parameter2, ..., auto parameterN);
    };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `parameter1-parameterN` | - | `N` constructor parameter | - |

```
Automatic constructor parameters deduction is limited to [BOOST_DI_CFG_CTOR_LIMIT_SIZE, which by defaults is set to 10.
```

```
Boost.DI is not able to distinguish between ambiguous constructors with the same (longest) amount of parameters.
```

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_direct.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_aggregate.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_multiple_constructors.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
BOOST_DI_INJECT
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_multiple_constructors.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_ambigious_constructors_via_inject.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_ambigious_constructors_via_inject_traits.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::ctor_traits
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_ambigious_constructors_via_ctor_traits.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

###Annotations

```cpp
(named = name)
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annotated_constructor_injection.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annotated_constructor_injection_with_constructor_definition.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annotated_constructor_injection_with_ctor_traits.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annoteded_constructor_injection_with_the_same_names.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

###Scopes

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_custom.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::deduce (default)
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_deduce_default.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::instance (di::bind<>.to(value))
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_instance.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::singleton
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_singleton.cpp)

***Example***
![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::unique
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_unique.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

###Modules

```cpp
auto module = [] { return di::make_injector(...); };
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module_exposed_type.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module_exposed_many_types.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module_exposed_annotated_type.cpp)

***Example***
![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

###Providers

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/providers_heap_no_throw.cpp)

***Example***
![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::providers::stack_over_heap (default)
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

***Example***

&nbsp;

---

```cpp
di::providers::heap
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

***Example***

&nbsp;

---

###Policies

```cpp
di::policies::constructible
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/policies_print_types.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/policies_print_type_extended.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/policies_constructible_global.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

###Concepts

```cpp
di::concepts::boundable
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::concepts::callable
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::concepts::configurable
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::concepts::creatable
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::concepts::providable
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::concepts::scopable
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

###Configuration

```cpp
di::config
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---
