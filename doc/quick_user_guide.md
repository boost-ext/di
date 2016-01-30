![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/injector_empty.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_interface_to_implementation.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_type_to_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_deduce_type_to_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_type_to_compile_time_value.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_multiple_interfaces.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/bind_cross_platform.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_direct.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_aggregate.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_multiple_constructors.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_ambigious_constructors_via_ctor_traits.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_ambigious_constructors_via_inject.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/constructor_injection_ambigious_constructors_via_inject_traits.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annotated_constructor_injection.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annotated_constructor_injection_with_constructor_definition.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annotated_constructor_injection_with_ctor_traits.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/annoteded_constructor_injection_with_the_same_names.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_deduce_default.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_unique.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_singleton.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_instance.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/scopes_custom.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module_exposed_type.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module_exposed_many_types.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/module_exposed_annotated_type.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/providers_heap_no_throw.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/policies_print_types.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/policies_print_type_extended.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/quick_user_guide/policies_constructible_global.cpp)

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

| Type/Scope | unique | singleton | external |
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
