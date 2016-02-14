* [Injector](#injector)
* [Bindings](#bindings)
* [Injections](#injections)
* [Annotations](#annotations)
* [Scopes](#scopes)
* [Modules](#modules)
* [Providers](#providers)
* [Policies](#policies)

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

* [**Injector**](user_guide.md#injector)

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/injector_empty.cpp)

* [**Bindings**](user_guide.md#bindings)

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_interface_to_implementation.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_type_to_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_deduce_type_to_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_type_to_compile_time_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_multiple_interfaces.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/bind_cross_platform.cpp)

* [**Injections**](user_guide.md#injections)

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_direct.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_aggregate.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_multiple_constructors.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_ambigious_constructors_via_ctor_traits.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_ambigious_constructors_via_inject.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/constructor_injection_ambigious_constructors_via_inject_traits.cpp)

* [**Annotations**](user_guide.md#annotations)

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/annotated_constructor_injection.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/annotated_constructor_injection_with_constructor_definition.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/annotated_constructor_injection_with_ctor_traits.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/annoteded_constructor_injection_with_the_same_names.cpp)

* [**Scopes**](user_guide.md#scopes)

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/scopes_deduce_default.cpp)

| Type | Deduced scope |
|------|-------|
| T | unique |
| T& | singleton |
| const T& | unique (temporary)/singleton |
| T* | unique (ownership transfer) |
| const T* | unique (ownership transfer) |
| T&& | unique |
| unique\_ptr<T> | unique |
| shared\_ptr<T> | singleton |
| weak\_ptr<T> | singleton |

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/scopes_unique.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/scopes_singleton.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/scopes_instance.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/scopes_custom.cpp)

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

* [**Modules**](user_guide.md#modules)

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module_exposed_type.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module_exposed_many_types.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module_exposed_annotated_type.cpp)

* [**Providers**](user_guide.md#providers)

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/providers_heap_no_throw.cpp)

* [**Policies**](user_guide.md#policies)

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/policies_print_types.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/policies_print_type_extended.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/policies_constructible_global.cpp)

