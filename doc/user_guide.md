* [Injector](#injector)
    * [di::make_injector](#di_make_injector)
* [Bindings](#bindings)
    * [di::bind](#di_bind)
* [Injections](#injections)
    * [automatic (default)](#di_automatic)
    * [BOOST_DI_INJECT](#BOOST_DI_INJECT)
    * [BOOST_DI_INJECT_TRAITS](#BOOST_DI_INJECT_TRAITS)
    * [di::inject](#di_inject)
    * [di::ctor_traits](#di_ctor_traits)
* [Annotations](#annotations)
    * [(named = name)](#di_named)
* [Scopes](#scopes)
    * [di::deduce (default)](#di_deduce)
    * [di::instance (di::bind<>.to(value))](#di_instance)
    * [di::singleton](#di_singleton)
    * [di::unique](#di_unique)
* [Modules](#modules)
* [Providers](#providers)
    * [di::providers::stack_over_heap (default)](#di_stack_over_heap)
    * [di::providers::heap](#di_heap)
* [Policies](#policies)
    * [di::policies::constructible](#di_constructible)
* [Concepts](#concepts)
    * [di::concepts::boundable](#di_boundable)
    * [di::concepts::callable](#di_callable)
    * [di::concepts::configurable](#di_configurable)
    * [di::concepts::creatable](#di_creatable)
    * [di::concepts::providable](#di_providable)
    * [di::concepts::scopable](#di_scopable)
* [Configuration](#configuration)
    * [di::config](#di_config)

---

> Let's assume all examples below include `boost/di.hpp` header and define `di` namespace alias.
```cpp
#include <boost/di.hpp>
namespace di = boost::di;
//
struct i1 { virtual ~i1() = default; virtual void dummy1() = 0; };
struct i2 { virtual ~i2() = default; virtual void dummy2() = 0; };
struct impl1 : i1 { void dummy1() override { } };
struct impl2 : i2 { void dummy2() override { } };
struct impl : i1, i2 { void dummy1() override { } void dummy2() override { } };
```

###Injector

<a id="di_make_injector"></a>

--- ***di::make_injector*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Creates [injector] type.

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
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/injector_empty.cpp)
***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

<br /><hr />

###Bindings

Bindings define dependencies configuration which basically means what types will be created
and what values will be passed into them.

<a id="di_bind"></a>
--- ***di::bind*** ---

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
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_interface_to_implementation.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_type_to_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_deduce_type_to_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_type_to_compile_time_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_multiple_interfaces.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_type_override.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_cross_platform.cpp)
***Example***

![CPP(BTN)](Run_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/bindings.cpp)
![CPP(BTN)](Run_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/fwd_bindings.cpp)
![CPP(BTN)](Run_Dynamic_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/dynamic_bindings.cpp)
![CPP(BTN)](Run_Multiple_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/multiple_bindings.cpp)
![CPP(BTN)](Run_Constructor_Bindings_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/bindings/constructor_bindings.cpp)

<br /><br /><br /><hr />

###Injections

*Constructor Injection* is the most powerful of available injections.
It guarantees initialized state of data members. Boost.DI constructor injection is achieved without any additional work from the user.

<a id="di_automatic"></a>
--- ***automatic (default)*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Boost.DI will deduce the best available constructor to be used for injection - unique constructor with the longest parameter list.
If the default behavior should be changed constructor has to be explicitly marked with [BOOST_DI_INJECT] or [BOOST_DI_INJECT_TRAITS].

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
Automatic constructor parameters deduction is limited to [BOOST_DI_CFG_CTOR_LIMIT_SIZE], which by default is set to 10.
</div>

***Semantics***

    class T {
    public:
      T(auto parameter1, auto parameter2, ..., auto parameterN);
    };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `parameter1-parameterN` | - | `N` constructor parameter | - |

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
Boost.DI is not able to distinguish between ambiguous constructors with the same (longest) amount of parameters.
</div>

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_direct.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_aggregate.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_multiple_constructors.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_ambigious_constructors_via_vaargs.cpp)
***Example***

![CPP(BTN)](Run_Automatic_Injection_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/automatic_injection.cpp)
![CPP(BTN)](Run_Constructor_Signature_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/constructor_signature.cpp)
![CPP(BTN)](Run_Concepts_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/injections/concepts.cpp)
![CPP(BTN)](Run_Assisted_Injection_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/injections/assisted_injection.cpp)
![CPP(BTN)](Run_Lazy_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/injections/lazy.cpp)
![CPP(BTN)](Run_Named_Parameters_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/injections/named_parameters.cpp)
![CPP(BTN)](Run_XML_Injection_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/injections/xml_injection.cpp)

<br /><br /><br /><hr />

<a id="BOOST_DI_INJECT"></a>
--- ***BOOST_DI_INJECT*** ---

***Header***

    #include <boost/di.hpp>

***Description***

BOOST_DI_INJECT is a macro definition used to explicitly say Boost.DI which constructor should be used as well as in order to annotate types - see [annotations] for further reding.
When class has more than one constructor Boost.DI will by default choose the one with the longest parameter list.
In case of constructors ambiguity Boost.DI is not able to choose the best one.
Then BOOST_DI_INJECT become handy to point which constructor should be used.

***Semantics***

    struct T {
        BOOST_DI_INJECT(T, ...) { }
    };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `T` | - | Class type | - |
| `...` | - | `T` constructor parameters | - |


<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
BOOST_DI_INJECT constructor parameters is limited to [BOOST_DI_CFG_CTOR_LIMIT_SIZE], which by default is set to 10.
</div>

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_ambigious_constructors_via_BOOST_DI_INJECT.cpp)
***Example***

![CPP(BTN)](Run_Constructor_Injection_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/constructor_injection.cpp)

<br /><hr />

<a id="BOOST_DI_INJECT_TRAITS"></a>
--- ***BOOST_DI_INJECT_TRAITS*** ---

***Header***

    #include <boost/di.hpp>

***Description***

BOOST_DI_INJECT_TRAITS is a macro definition used to define constructor traits.

***Semantics***

    struct T {
      BOOST_DI_INJECT_TRAITS(...) { }
      T(...) { }
    };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `...` | - | `T` constructor parameters | - |

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
BOOST_DI_INJECT_TRAITS constructor parameters is limited to [BOOST_DI_CFG_CTOR_LIMIT_SIZE], which by default is set to 10.
</div>

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_ambigious_constructors_via_BOOST_DI_INJECT_TRAITS.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_default_values.cpp)
***Example***

![CPP(BTN)](Run_Constructor_Injection_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/constructor_injection.cpp)

<br /><hr />

<a id="di_inject"></a>
--- ***di::inject*** ---

***Header***

    #include <boost/di.hpp>

***Description***

di::inject informs Boost.DI about constructor parameters. Useful for generated/generic classes as it doesn't have constructor parameters size limitations.

***Semantics***

    struct T {
      using boost_di_inject__ = di::inject<...>;
      T(...) {}
    };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `...` | - | `T` constructor parameters | - |

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
di::inject has no limitations if it comes to constructor parameters, however, named parameters are not allowed.
</div>

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_ambigious_constructors_via_inject.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_long_parameter_list.cpp)
***Example***

![CPP(BTN)](Run_Constructor_Injection_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/constructor_injection.cpp)

<br /><hr />

<a id="di_ctor_traits"></a>
--- ***di::ctor_traits*** ---

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_ambigious_constructors_via_ctor_traits.cpp)
***Example***

![CPP(BTN)](Run_Constructor_Injection_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/constructor_injection.cpp)

<br /><hr />

###Annotations

Annotations are intrusive, additional informations specified along with the type in order to refer to given type by the
annotation instead of type it self. Useful, when there are more than one type of the same parameters in constructor parameters.

<a id="di_named"></a>
--- ***(named = name)*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Named parameters are useful when constructor has more parameters of the same type.

```cpp
  T(int value1, int value2);
```

In order to inject proper values into `value1` and `value2` they have to be distinguished somehow.
Boost.DI solution for that problem are annotations.

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
Annotations might be set only when constructor is selected using BOOST_DI_INJECT or BOOST_DI_INJECT_TRAITS.
</div>

***Semantics***

    auto Name = []{}; // just an object

    struct T {
      BOOST_DI_INJECT(T, (named = Name) type type_name [= default_value], ...);
    };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `Name` | - | Object representing named type | - |

***Example***

    BOOST_DI_INJECT(T, (named = value_1) int value1, (named = value_2) int value2);

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
Implementation of constructor doesn't require annotations, which means implementation won't be affected by annotations.
</div>

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/annotated_constructor_injection.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/annotated_constructor_injection_with_constructor_definition.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/annotated_constructor_injection_with_ctor_traits.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/annoteded_constructor_injection_with_the_same_names.cpp)
***Example***

![CPP(BTN)](Run_Annotations_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/annotations.cpp)

<br /><hr />

###Scopes

***Header***

    #include <boost/di.hpp>

***Description***

Scopes are responsible for creating and maintaining life time of dependencies.
If no scope will be given, deduce scope will be assumed.

***Semantics***

    template <class TExpected, class TGiven>
    struct scope {
      template <class T>
      using is_referable;

      template <class T, class TName, class TProvider>
      static auto try_create(const TProvider&);

      template <class T, class TName, class TProvider>
      auto create(const TProvider&);
    };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `TExpected` | - | 'Interface' type | - |
| `TGiven` | - | 'Implementation' type | - |
| `is_referable<T\>` | - | Verifies whether scope value might be converted to a reference | std::true_type/std::false_type |
| `try_create<T, TName, TProvider\>` | [providable]<TProvider\> | Verifies whether type might be created | std::true_type/std::false_type |
| `create<T, TName, TProvider\>` | [providable]<TProvider\> | Creates type might be created | `T` |

| Type/Scope | unique | singleton | instance |
|------------|--------|--------|-----------|---------|----------|
| T | ✔ | - | ✔ |
| T& | - | ✔  | ✔ |
| const T& | ✔ (temporary) | ✔ | ✔ |
| T* (transfer ownership) | ✔ | - | - | - | ✔ |
| const T* | ✔ | - | ✔ |
| T&& | ✔ | - | - |
| unique\_ptr<T> | ✔ |  - | ✔ |
| shared\_ptr<T> | ✔ | ✔ | ✔ |
| weak\_ptr<T> | - | ✔ | ✔ |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/scopes_custom.cpp)
***Example***

![CPP(BTN)](Run_Custom_Scope_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/custom_scope.cpp)
![CPP(BTN)](Run_Scoped_Scope_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/scopes/scoped_scope.cpp)
![CPP(BTN)](Run_Session_Scope_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/scopes/session_scope.cpp)
![CPP(BTN)](Run_Shared_Scope_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/scopes/shared_scope.cpp)

<br /><br /><br /><hr />

<a id="di_deduce"></a>
--- ***di::deduce (default)*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Default scope which will be converted to one of the scopes depending on the type.

| Type | deduce |
|------|-------|
| T | [unique] |
| T& | [singleton] |
| const T& | [unique] (temporary)/singleton |
| T* | [unique] (ownership transfer) |
| const T* | [unique] (ownership transfer) |
| T&& | [unique] |
| unique\_ptr<T> | [unique] |
| shared\_ptr<T> | [singleton] |
| weak\_ptr<T> | [singleton] |

***Semantics***

    namespace scopes {
      struct deduce {
        template <class TExpected, class TGiven>
        struct scope {
          template <class T>
          using is_referable;

          template <class T, class TName, class TProvider>
          static auto try_create(const TProvider&);

          template <class T, class TName, class TProvider>
          auto create(const TProvider&);
        };
      };
    }

    scopes::deduce deduce;

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `TExpected` | - | 'Interface' type | - |
| `TGiven` | - | 'Implementation' type | - |
| `is_referable<T\>` | - | Verifies whether scope value might be converted to a reference | std::true_type/std::false_type |
| `try_create<T, TName, TProvider\>` | [providable]<TProvider\> | Verifies whether type might be created | std::true_type/std::false_type |
| `create<T, TName, TProvider\>` | [providable]<TProvider\> | Creates type might be created | `T` |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/scopes_deduce_default.cpp)
***Example***

![CPP(BTN)](Run_Deduce_Scope_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/deduce_scope.cpp)
![CPP(BTN)](Run_Scopes_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/scopes.cpp)

<br /><hr />

<a id="di_instance"></a>
--- ***di::instance (di::bind<>.to(value))*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Scope representing values - passed by user. The life time of the object depends on the user.
Boost.DI is not managing life time of passed objects, however values and strings will be copied and managed by the library.

| Type | instance |
| ---- | -------- |
| T | ✔ |
| T& | - |
| const T& | ✔ (temporary) | ✔ |
| T* (transfer ownership) | ✔ |
| const T* | ✔ |
| T&& | ✔ |
| unique\_ptr<T> | ✔ |
| shared\_ptr<T> | ✔ |
| weak\_ptr<T> | - |

***Semantics***

    namespace scopes {
      struct instance {
        template <class TExpected, class TGiven>
        struct scope {
          template <class T>
          using is_referable;

          template <class T, class TName, class TProvider>
          static auto try_create(const TProvider&);

          template <class T, class TName, class TProvider>
          auto create(const TProvider&);
        };
      };
    }

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `TExpected` | - | 'Interface' type | - |
| `TGiven` | - | 'Implementation' type | - |
| `is_referable<T\>` | - | Verifies whether scope value might be converted to a reference | std::true_type/std::false_type |
| `try_create<T, TName, TProvider\>` | [providable]<TProvider\> | Verifies whether type might be created | std::true_type/std::false_type |
| `create<T, TName, TProvider\>` | [providable]<TProvider\> | Creates type might be created | `T` |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/scopes_instance.cpp)
***Example***

![CPP(BTN)](Run_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/bindings.cpp)
![CPP(BTN)](Run_Scopes_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/scopes.cpp)

<br /><hr />

<a id="di_singleton"></a>
--- ***di::singleton*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Scope representing shared value between all instances and between threads.
Singleton scope will be deduced in case of shared_ptr or weak_ptr.

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
Singleton scope will convert between std::shared_ptr and boost::shared_ptr if required.
</div>

| Type | singleton |
| ---- | --------- |
| T | ✔ |
| T& | - |
| const T& | ✔ (temporary) | ✔ |
| T* (transfer ownership) | ✔ |
| const T* | ✔ |
| T&& | ✔ |
| unique\_ptr<T> | ✔ |
| shared\_ptr<T> | ✔ |
| weak\_ptr<T> | - |

***Semantics***

    namespace scopes {
      struct singleton {
        template <class TExpected, class TGiven>
        struct scope {
          template <class T>
          using is_referable;

          template <class T, class TName, class TProvider>
          static auto try_create(const TProvider&);

          template <class T, class TName, class TProvider>
          auto create(const TProvider&);
        };
      };
    }

    scopes::singleton singleton;

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `TExpected` | - | 'Interface' type | - |
| `TGiven` | - | 'Implementation' type | - |
| `is_referable<T\>` | - | Verifies whether scope value might be converted to a reference | std::true_type/std::false_type |
| `try_create<T, TName, TProvider\>` | [providable]<TProvider\> | Verifies whether type might be created | std::true_type/std::false_type |
| `create<T, TName, TProvider\>` | [providable]<TProvider\> | Creates type might be created | `T` |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/scopes_singleton.cpp)
***Example***

![CPP(BTN)](Run_Scopes_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/scopes.cpp)
![CPP(BTN)](Run_Eager_Singletons_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/eager_singletons.cpp)

<br /><hr />

<a id="di_unique"></a>
--- ***di::unique*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Scope representing unique/per request value.

| Type | unique |
| ---- | ------ |
| T | ✔ |
| T& | - |
| const T& | ✔ (temporary) | ✔ |
| T* (transfer ownership) | ✔ |
| const T* | ✔ |
| T&& | ✔ |
| unique\_ptr<T> | ✔ |
| shared\_ptr<T> | ✔ |
| weak\_ptr<T> | - |

***Semantics***

    namespace scopes {
      struct unique {
        template <class TExpected, class TGiven>
        struct scope {
          template <class T>
          using is_referable;

          template <class T, class TName, class TProvider>
          static auto try_create(const TProvider&);

          template <class T, class TName, class TProvider>
          auto create(const TProvider&);
        };
      };
    }

    scopes::unique unique;

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `TExpected` | - | 'Interface' type | - |
| `TGiven` | - | 'Implementation' type | - |
| `is_referable<T\>` | - | Verifies whether scope value might be converted to a reference | std::true_type/std::false_type |
| `try_create<T, TName, TProvider\>` | [providable]<TProvider\> | Verifies whether type might be created | std::true_type/std::false_type |
| `create<T, TName, TProvider\>` | [providable]<TProvider\> | Creates type might be created | `T` |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/scopes_unique.cpp)
***Example***

![CPP(BTN)](Run_Scopes_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/scopes.cpp)

<br /><hr />

###Modules

<a id="di_module"></a>
```cpp
auto module = [] { return di::make_injector(...); };
```

***Header***

    #include <boost/di.hpp>

***Description***

Modules allow to split the configuration into smaller injectors.
Module might be installed by passing it into [make_injector].

***Semantics***

    auto module = di::make_injector(...);

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module_exposed_type.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module_exposed_many_types.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module_exposed_annotated_type.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module_exposed_complex_types.cpp)
***Example***

![CPP(BTN)](Run_Modules_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/modules.cpp)

<br /><hr />

###Providers

***Header***

    #include <boost/di.hpp>

***Description***

Providers are responsible for creating objects using given configuration.

***Semantics***

    namespace type_traits {
      struct direct; // T(...)
      struct uniform; // T{...}
      struct heap; // new T
      struct stack; // T
    }

    namespace providers {
      class provider {
        public:
          template <class T, class... TArgs>
          struct is_creatable;

          template <
            class T
          , class TInit // type_traits::direct/type_traits::uniform
          , class TMemory // type_traits::heap/type_traits::stack
          , class... TArgs
          > auto get(const TInit&, const TMemory&, TArgs&&... args) const;
      };
    }

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `is_creatable<T, TArgs...\>` | [creatable]<TArgs...\> | Verify whether `T` is creatable with `TArgs...` | `T` |
| `get<T, TInitialization, TMemory, TArgs\>(const TInitialization&, const TMemory&, TArgs&&...)` | `TInitialization` is direct\|uniform && `TMemory` is heap\|stack | Creates type `T` with `TArgs...` | `T` |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/providers_heap_no_throw.cpp)
***Example***

![CPP(BTN)](Run_Custom_Provider_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/custom_provider.cpp)
![CPP(BTN)](Run_Pool_Provider_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/pool_provider.cpp)

<br /><hr />

<a id="di_stack_over_heap"></a>
--- ***di::providers::stack_over_heap (default)*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Creates objects on the stack whenever possible, otherwise on the heap.

***Semantics***

    namespace providers {
      class stack_over_heap {
        public:
          template <class T, class... TArgs>
          struct is_creatable;

          template <
            class T
          , class TInit // type_traits::direct/type_traits::uniform
          , class TMemory // type_traits::heap/type_traits::stack
          , class... TArgs
          > auto get(const TInit&, const TMemory&, TArgs&&... args) const;
      };
    }

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `is_creatable<T, TArgs...\>` | [creatable]<TArgs...\> | Verify whether `T` is creatable with `TArgs...` | `T` |
| `get<T, TInitialization, TMemory, TArgs\>(const TInitialization&, const TMemory&, TArgs&&...)` | `TInitialization` is direct\|uniform && `TMemory` is heap\|stack | Creates type `T` with `TArgs...` | `T` |

| Type | `TMemory` |
|------|-------|
| T | stack |
| T& | stack |
| const T& | stack |
| T* | stack |
| const T* | stack |
| T&& | stack |
| unique\_ptr<T> | stack |
| shared\_ptr<T> | stack |
| weak\_ptr<T> | stack |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/providers_stack_over_heap.cpp)
***Example***

![CPP(BTN)](Run_Custom_Provider_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/custom_provider.cpp)
![CPP(BTN)](Run_Pool_Provider_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/pool_provider.cpp)

<br /><hr />

<a id="di_heap"></a>
--- ***di::providers::heap*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Basic provider creates objects on the heap.

***Semantics***

    namespace providers {
      class stack_over_heap {
        public:
          template <class T, class... TArgs>
          struct is_creatable;

          template <
            class T
          , class TInit // type_traits::direct/type_traits::uniform
          , class TMemory // type_traits::heap/type_traits::stack
          , class... TArgs
          > auto get(const TInit&, const TMemory&, TArgs&&... args) const;
      };
    }

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `is_creatable<T, TArgs...\>` | [creatable]<TArgs...\> | Verify whether `T` is creatable with `TArgs...` | `T` |
| `get<T, TInitialization, TMemory, TArgs\>(const TInitialization&, const TMemory&, TArgs&&...)` | `TInitialization` is direct\|uniform && `TMemory` is heap\|stack | Creates type `T` with `TArgs...` | `T` |

| Type | `TMemory` |
|------|-------|
| T | stack |
| T& | stack |
| const T& | stack |
| T* | stack |
| const T* | stack |
| T&& | stack |
| unique\_ptr<T> | stack |
| shared\_ptr<T> | stack |
| weak\_ptr<T> | stack |

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/providers_heap.cpp)
***Example***

![CPP(BTN)](Run_Custom_Provider_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/custom_provider.cpp)
![CPP(BTN)](Run_Pool_Provider_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/pool_provider.cpp)
![CPP(BTN)](Run_Mocks_Provider_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/providers/mocks_provider.cpp)

<br /><br /><br /><hr />

###Policies

***Header***

    #include <boost/di.hpp>

***Description***

Policies operates on dependencies in order to limit allowed behaviour or visit created types during run-time.
Policies are set up via [di::config].

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
By default Boost.DI has no policies enabled.
</div>

***Semantics***

  template <class... TPolicies> requires callable<TPolicies...>
  auto make_policies(TPolicies... args) noexcept;

  struct config : di::config {
    static auto policies(...) noexcept { return make_policies(...); }
  };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `make_policies<TPolicies...\>` | [callable]<TPolicies...\> | Creates policies | [callable] list |

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
In order for injector to verify policies they have to be created using di::config and passed via `TConfig` in make_injector or set globally via BOOST_DI_CFG.
</div>

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/policies_print_types.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/policies_print_type_extended.cpp)
***Example***

![CPP(BTN)](Run_Custom_Policy_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/custom_policy.cpp)
![CPP(BTN)](Run_Types_Dumper_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/policies/types_dumper.cpp)
![CPP(BTN)](Run_UML_Dumper_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/policies/uml_dumper.cpp)

<br /><br /><br /><hr />

<a id="di_constructible"></a>
--- ***di::policies::constructible*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Policy limits constructor parameters to explicitly allowed.

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
By default constructible policy disables creation of any constructor parameters.
</div>

***Semantics***

    namespace policies {
      struct _ { }; // placeholder

      template<class T>
      struct is_bound; // true when type is bound with 'di::bind<T>'

      template <class T>
      struct is_injected; // true when type is injected using 'BOOST_DI_INJECT' or is 'fundamental'

      template<class T>
      auto constructible(const T&) noexcept;
    }

    namespace operators {
      template<class X>
      inline auto operator!(const X&)

      template<class X, class Y>
      inline auto operator&&(const X&, const Y&);

      template<class X, class Y>
      inline auto operator||(const X&, const Y&);
    }

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `is_bound<T\>` | - | Verify whether type `T` is bound | true_type/false_type |
| `is_injected<T\>` | - | Verify whether type `T` is injected via [BOOST_DI_INJECT] | true_type/false_type |


<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
In order to allow logic operators using namespace boost::di::policies::operators has to be used
</div>

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/policies_constructible_local.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/policies_constructible_global.cpp)

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
STL type traits are supported and might be combined with Boost.DI traits in order to limit constructor types
For example, std::is_same<_, int>{} || std::is_constructible<_, int, int>{} || std::is_base_of<int, _>{}, etc...
</div>

***Example***

![CPP(BTN)](Run_Custom_Policy_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/custom_policy.cpp)
![CPP(BTN)](Run_Types_Dumper_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/policies/types_dumper.cpp)
![CPP(BTN)](Run_UML_Dumper_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/policies/uml_dumper.cpp)

<br /><br /><br /><hr />

###Concepts

Concepts are types constraints which ensure that only given types which are satisfied by the constraint will be allowed.
If type doesn't satisfy the concept short and descriptive error message is provided.

<a id="di_boundable"></a>
--- ***di::concepts::boundable*** ---

***Header***

    #include <boost/di.hpp>

***Description***

  Bindings type requirement.

***Synopsis***

    template <class TExpected, class TGiven>
    concept bool boundable() {
      return is_complete<TExpected>()
          && is_complete<TGiven>()
          && (is_base_of<TExpected, TGiven>() || is_convertible<TGiven, TExpected>());
    }

    template <class... Ts>
    concept bool boundable() {
      return is_supported<Ts>()... 
          && is_movable<Ts>()...
          && (is_base_of<injector, Ts>()... || is_base_of<dependency, Ts>()...);
    }

***Semantics***

    boundable<T>
    boundable<Ts...>

| Expression | Description | Returns |
| ---------- | ----------- | ------- |
| `Ts...` | Bindings to be verified | true_type if constraint is satisfied, `error` otherwise |

***Example***

| Error | `type<T>::has_disallowed_qualifiers` |
| ---------- | ----------- |
| Description | type `T` has disallowed qualifiers |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/boundable_type_has_disallowed_qualifiers.cpp) |

| Error | `type<T>::is_abstract` |
| ---------- | ----------- |
| Description | type `T` is abstract |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL` | 0, 1 -> no additional info, 2 -> info about why type `T` is abstract |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/boundable_type_is_abstract.cpp) |

| Error | `type<T>::is_not_related_to` |
| ---------- | ----------- |
| Description | type `T` is not related to type `U` |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/boundable_type_is_not_related_to.cpp) |

| Error | `type<T>::is_bound_more_than_once` |
| ---------- | ----------- |
| Description | type `T` is bound more than once |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/boundable_type_is_bound_more_than_once.cpp) |

| Error | `type<T>::is_neither_a_dependency_nor_an_injector` |
| ---------- | ----------- |
| Description | type `T` is neither a dependency nor an injector |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/boundable_type_is_neither_a_dependency_nor_an_injector.cpp) |

---

<a id="di_callable"></a>
--- ***di::concepts::callable*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Policies type requirement.

***Synopsis***

    template <class T>
    concept bool callable() {
      return requires(T object) {
        { object(...) };
      }
    }

***Semantics***

    callable<T>

***Example***

| Error | `policy<TPolicy>::requires_<call_operator>` |
| ---------- | ----------- |
| Description | policy `TPolicy` requires a call operator |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/callable_requires_call_operator.cpp) |

---

<a id="di_configurable"></a>
--- ***di::concepts::configurable*** ---

***Header***

    #include <boost/di.hpp>

***Description***

***Synopsis***

    template <class T>
    concept bool configurable() {
      return requires(T object) {
        return providable<decltype(T::provider(...))> && callable<decltype(T::policies(...))>();
      }
    }

***Semantics***

    configurable<T>

***Example***

| Error | `config<TConfig>::requires_<provider<providable_type (...)>>` |
| ---------- | ----------- |
| Description | config `T` requires only providable and callable types |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/configurable_requires_callable_and_providable.cpp) |

---

<a id="di_creatable"></a>
--- ***di::concepts::creatable*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Type creation requirement.

***Synopsis***

    template <class T, class... TBindings>
    concept bool creatable() {
      return is_recursively_constructible<T, TBindings...>();
    }

***Semantics***

    creatable<T, TBindings...>

***Example***

| Error | `abstract_type<T>::is_not_bound` |
| ---------- | ----------- |
| Description | config `T` requires only providable and callable types |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL` | 0 -> 'constraint not satisfied', 1 -> (0) + abstract type is not bound, 2 -> (1) + creation tree |
| Suggestion  | 'type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?' |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/creatable_abstract_type_is_not_bound.cpp) |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/creatable_expose_abstract_type_is_not_bound.cpp) |

| Error | `type<T>::has_ambiguous_number_of_constructor_parameters::given<Given>::expected<Expected>` |
| ---------- | ----------- |
| Description | type `T` has ambiguous number of constructor parameters where `Given` were provided but `Expected` were expected |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL` | 0 -> 'constraint not satisfied', 1 -> (0) + abstract type is not bound, 2 -> (1) + creation tree |
| Suggestion  | 'verify BOOST_DI_INJECT_TRAITS or di::ctor_traits' |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/creatable_type_has_ambiguous_number_of_constructor_parameters.cpp) |

| Error | `type<T>::has_to_many_constructor_parameters::max<Max>` |
| ---------- | ----------- |
| Description | type `T` has to many constructor parameter where maximum number is `Max` |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL` | 0 -> 'constraint not satisfied', 1 -> (0) + abstract type is not bound, 2 -> (1) + creation tree |
| Suggestion  | 'increase BOOST_DI_CFG_CTOR_LIMIT_SIZE value or reduce number of constructor parameters' |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/creatable_type_has_to_many_constructor_parameters.cpp) |

| Error | `scoped<TScope>::is_not_convertible_to<T>` |
| ---------- | ----------- |
| Description | scope `TScope` is not convertible to type `T` |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL` | 0 -> 'constraint not satisfied', 1 -> (0) + abstract type is not bound, 2 -> (1) + creation tree |
| Suggestion  | 'scoped object is not convertible to the requested type, did you mistake the scope: 'di::bind<T>.in(scope)'?' |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/creatable_scoped_is_not_convertible_to.cpp) |

| Error | `scoped<instance>::is_not_convertible_to<T>` |
| ---------- | ----------- |
| Description | instance is not convertible to type `T` |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL` | 0 -> 'constraint not satisfied', 1 -> (0) + abstract type is not bound, 2 -> (1) + creation tree |
| Suggestion  | 'instance is not convertible to the requested type, verify binding: 'di::bind<T>.to(value)'?' |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/creatable_instance_is_not_convertible_to.cpp) |

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
Suggestions are not supported/displayed by MSVC-2015.
</div>

---

<a id="di_providable"></a>
--- ***di::concepts::providable*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Provider type requirement.

***Synopsis***

    template <class T>
    concept bool providable() {
      return requires(T object) {
        { object.template get<_>(direct/uniform{}, stack/heap{}, ...) };
        { object.template is_creatable<_>(direct/uniform{}, stack/heap{}, ...) };
      }
    }

***Semantics***

   providable<T>

***Example***

| Error | `provider<TProvider>::requires_<get>` |
| ---------- | ----------- |
| Description | provider `TProvider` requires `get` method |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/providable_requires_get.cpp) |

---

<a id="di_scopable"></a>
--- ***di::concepts::scopable*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Scope type requirement.

***Synopsis***

    template <class T>
    concept bool scopable() {
      return requires(T) {
        typename scope<_, _>::is_referable;
        { T::scope<_, _>{}.try_create() };
        { T::scope<_, _>{}.create() };
      }
    }

***Semantics***

   scopable<T>

***Example***

| Error | `scope<TScope>::requires_<create>` |
| ---------- | ----------- |
| Description | scope `TScope` requires `create` method |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/scopable_requires_create.cpp) |

---

###Configuration

<a id="di_config"></a>
--- ***di::config*** ---

***Header***

    #include <boost/di.hpp>

***Description***

Injector configuration.

***Synopsis***

    struct config {
      static auto provider(...) noexcept;
      static auto policies(...) noexcept;
    };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `provider()` | [providable] | Creates provider | [providable] |
| `policies()` | [callable] | Creates policies | [callable] |

| Expression | Description |
| ---------- | ----------- |
| `BOOST_DI_CFG` | Global configuration allows to customize provider and policies |

***Semantics***

    di::make_injector<config>(...)
    // or
    #define BOOST_DI_CFG config // change default
    di::make_injector(...)

***Test***
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/policies_constructible_global.cpp)
***Example***

![CPP(BTN)](Run_Configuration_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/configuration.cpp)

<br /><hr />

[boundable]: #di_boundable
[callable]: #di_callable
[configurable]: #di_configurable
[creatable]: #di_creatable
[providable]: #di_providable
[scopable]: #di_scopable
[injector]: #di_make_injector
[Configuration]: #di_config
[deduce]: #di_deduce
[instance]: #di_instance
[singleton]: #di_singleton
[unique]: #di_unique
[BOOST_DI_INJECT]: #BOOST_DI_INJECT
[BOOST_DI_INJECT_TRAITS]: #BOOST_DI_INJECT_TRAITS
[BOOST_DI_CFG_CTOR_LIMIT_SIZE]: #di_config
