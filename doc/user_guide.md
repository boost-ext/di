###Injector

```cpp
di::injector
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::make_injector
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

###Bindings

```cpp
di::bind
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_interface_to_implementation.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_type_to_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_deduce_type_to_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_type_to_compile_time_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_multiple_interfaces.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_cross_platform.cpp)

![CPP(BTN)](Run_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/bindings.cpp)
![CPP(BTN)](Run_Dynamic_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/dynamic_bindings.cpp)
![CPP(BTN)](Run_Multiple_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/multiple_bindings.cpp)

&nbsp;

---

###Injections

```cpp
automatic (default)
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_direct.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_aggregate.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_multiple_constructors.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_ambigious_constructors_via_inject.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_ambigious_constructors_via_inject_traits.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_ambigious_constructors_via_ctor_traits.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annotated_constructor_injection.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annotated_constructor_injection_with_constructor_definition.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annotated_constructor_injection_with_ctor_traits.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annoteded_constructor_injection_with_the_same_names.cpp)

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

###Scopes

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_custom.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_deduce_default.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_instance.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_singleton.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_unique.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module_exposed_type.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module_exposed_many_types.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module_exposed_annotated_type.cpp)

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

###Providers

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/providers_heap_no_throw.cpp)

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

***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---

```cpp
di::providers::heap
```

***Header***

    #include <boost/di.hpp>

***Description***

***Semantics***

***Example***

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/policies_print_types.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/policies_print_type_extended.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/policies_constructible_global.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)

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

***Example***

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

&nbsp;

---
