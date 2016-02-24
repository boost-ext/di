###Quick Start

* Get [boost/di.hpp](https://raw.githubusercontent.com/boost-experimental/di/cpp14/include/boost/di.hpp) header
```sh
wget https://raw.githubusercontent.com/boost-experimental/di/cpp14/include/boost/di.hpp
```

* Include the header and define `di` namespace alias
```cpp
#include "boost/di.hpp"
namespace di = boost::di;
```

* Compile with C++14 support
```sh
$CXX -std=c++14 ...
```

* To run tests
```sh
git clone https://github.com/boost-experimental/di && cd di && make
```

###Dependencies

* No external dependencies are required (neither STL nor Boost)

###Supported/Tested compilers

* [Clang-3.4+](https://travis-ci.org/boost-experimental/di)
* [GCC-5.2+](https://travis-ci.org/boost-experimental/di)
* [MSVC-2015+](https://ci.appveyor.com/project/krzysztof-jusiak/di)

###Configuration
| Macro                             | Description |
| --------------------------------- | ----------- |
| `BOOST_DI_VERSION`                | Current version of Boost.DI (ex. 1'0'0) |
| `BOOST_DI_CFG`                    | Global configuration allows to customize provider and policies (See [Config](user_guide.md#di_config)) |
| `BOOST_DI_CFG_CTOR_LIMIT_SIZE`    | Limits number of allowed consturctor parameters [0-10, default=10] (See [Injections](user_guide.md#injections)) |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL`  | Gives more information with error messages (See [Error messages](#error-messages)) |
| `BOOST_DI_NAMESPACE_BEGIN`        | `namespace boost { namespace di { inline namespace v_1_0_0 {` |
| `BOOST_DI_NAMESPACE_END`          | `}}}` |

###Exception Safety

* Boost.DI is not using exceptions internally and therefore might be compiled with `-fno-exceptions`.
* Check [User Guide](user_guide.md) to verify which API's are marked `noexcept`.

###Thread Safety

* Boost.DI is thread safe.

###Design

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

* In a nutshell

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
sturct provider {
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
    if (is_constructible<T, any_type<TInjector>()...>()) {
      return pair<direct, any_type<TInjector>...>{}; // T(any_type...)
    }
  }

  for (auto i = BOOST_DI_CFG_CTOR_LIMIT_SIZE; i >= 0; --i) {
    if (is_braces_constructible<T, any_type<TInjector>()...>()) {
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

###Performance

* **Run-time performance**
    * Environment
        * x86\_64 Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz GenuineIntel GNU/Linux
        * clang++3.4 -O2 -fno-exceptions / `gdb -batch -ex 'file ./a.out' -ex 'disassemble main'`

![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/performance/create_type_without_bindings.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/performance/create_type_with_bound_instance.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/performance/create_named_type.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/performance/create_bound_interface.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/performance/create_bound_interface_via_module.cpp)
![CPP(SPLIT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/performance/create_bound_interface_via_exposed_module.cpp)

* **Compile-time performance**
    * Environment
        * x86\_64 Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz GenuineIntel GNU/Linux
        * clang++3.4 -O2 -fno-exceptions

Boost.DI header           | Time [s]
--------------------------|----------
`#include <boost/di.hpp>` | 0.050s

```cpp
Legend:
    ctor    = raw constructor: T(int i, double d);
    inject  = inject constructor: BOOST_DI_INJECT(T, int i, double d);
    all     = all types exposed from module: auto configure();
    exposed = one type exposed from module: di::injector<T> configure();
```
[![small complexity](https://raw.githubusercontent.com/boost-experimental/di/cpp14/doc/images/small_complexity.png)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/doc/images/small_complexity.png)
```cpp
* 4248897537 instances created
* 132 different types
* 10 modules
```
---
[![medium complexity](https://raw.githubusercontent.com/boost-experimental/di/cpp14/doc/images/medium_complexity.png)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/doc/images/medium_complexity.png)
```cpp
* 1862039751439806464 instances created
* 200 different types
* 10 modules
```
---
[![big complexity](https://raw.githubusercontent.com/boost-experimental/di/cpp14/doc/images/big_complexity.png)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/doc/images/big_complexity.png)
```cpp
* 5874638529236910091 instances created
* 310 different types
* 100 different interfaces
* 10 modules
```

###Error messages

| Error | `type<T>::is_abstract` |
| ---------- | ----------- |
| Description | type `T` is abstract |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL` | 0, 1 -> no additional info, 2 -> info about why type `T` is abstract |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/boundable_type_is_abstract.cpp) |

| Error | `type<T>::is_bound_more_than_once` |
| ---------- | ----------- |
| Description | type `T` is bound more than once |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/boundable_type_is_bound_more_than_once.cpp) |

| Error | `abstract_type<T>::is_not_bound` |
| ---------- | ----------- |
| Description | config `T` requires only providable and callable types |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL` | 0 -> 'constraint not satisfied', 1 -> (0) + abstract type is not bound, 2 -> (1) + creation tree |
| Suggestion  | 'type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?' |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/creatable_abstract_type_is_not_bound.cpp) |
| Expression  | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/creatable_expose_abstract_type_is_not_bound.cpp) |

###Benchmarks

* **C++ Dependency Injection Libraries**

| Library | Link |
| ------- | ---- |
| dicpp | [https://bitbucket.org/cheez/dicpp](https://bitbucket.org/cheez/dicpp) |
| Fruit | [https://github.com/google/fruit](https://github.com/google/fruit) |
| Sauce | [https://github.com/phs/sauce](https://github.com/phs/sauce) |
| Wallaroo | [http://wallaroolib.sourceforge.net](http://wallaroolib.sourceforge.net) |
| Hypodermic | [https://github.com/ybainier/hypodermic](https://github.com/ybainier/hypodermic) |
| Infectorpp | [https://code.google.com/p/infectorpp](https://code.google.com/p/infectorpp) |
| Injeqt | [https://github.com/vogel/injeqt](https://github.com/vogel/injeqt) |
| Kangaru | [https://github.com/gracicot/kangaru](https://github.com/gracicot/kangaru) |
| CppInject | [https://github.com/admiyo/CppInject](https://github.com/admiyo/CppInject) |
| DiLite | [http://codebros.github.io/DiLite](http://codebros.github.io/DiLite) |

| **Library** | Boost.DI | [dicpp] | [Google.Fruit] |
| ----------- | -------- | ----- | ------------ |
| Version     | 1.0.0 | HEAD | (pre) 1.1.0 |
| Language    | C++14 | C++11 | C++11 |
| License     | Boost 1.0 | Boost 1.0 | Apache 2.0|
| Linkage     | header only | header only | library |
| Dependencies| - | boost system/boost thread | - |
| Approach    | compile-time | run-time | compile/run-time |
| Errors      | compile-time errors | exceptions | compile-time errors/exceptions |

| Library | Bind Interface | Performance |
| ------- | -------------- | ----------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/di.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/di.cpp) |
| [dicpp] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dicpp.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dicpp.cpp) |
| [Google.Fruit] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/fruit.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/fruit.cpp) |

| Library | Bind Value | Performance |
| ------- | ---------- | ----------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/di.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/di.cpp) |
| [dicpp] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dicpp.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dicpp.cpp) |
| [Google.Fruit] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/fruit.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/fruit.cpp) |

| Library | Missing Binding Error |
| ------- | ---------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/di.cpp) |
| [dicpp] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/dicpp.cpp) |
| [Google.Fruit] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/fruit.cpp) |

---

| Test | Types | Interfaces | Modules | Instances |
| ---- | ----- | ---------- | ------- | --------- |
| [Create Simple](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/create_simple) | 101 | 0 | 1 | 699150338 |

| Clang-3.7 -O2             | Boost.DI | [dicpp] | [Google.Fruit] |
| ------------------------- | -------- | ------- | -------------- |
| Compilation time          | 0.707s   | 8.226s  | 6.724s (-DFRUIT_NO_LOOP_CHECK) |
| Execution time            | 0.002s   | -       | -      |
| Memory usage (stack/heap) | 1b/0b    | -       | -      |
| Executable size           | 4.2K     | 119K    | 892kb  |

---

| Test              | Types | Interfaces | Modules | Instances |
| ----------------- | ----- | ---------- | ------- | --------- |
| [Create Complex](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/create_complex) | 211 | 100 | 1 | 5874638529236910091 |

| Clang-3.7 -O2             | Boost.DI | [dicpp]    | [Google.Fruit] |
| ------------------------- | -------- | ---------- | -------------- |
| Compilation time          | 3.082s   | 1m14.080s  | 34.461s (-DFRUIT_NO_LOOP_CHECK) |
| Execution time            | 0.002s   | -          | -    |
| Memory usage (stack/heap) | 1b/0b    | -          | -    |
| Executable size           | 72kb     | 4.4M       | 5.1M |

* To run benchmarks
```sh
cd benchmark && make
```
---

* **Dependency Injection Libraries**

| **Library** | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| ----------  | -------- | ------------ | ------- | ------- |
| Language    | C++14    | Java | Java | C# |
| Version     | 1.0.0    | 4.0 | 2.0.2 | 3.0.2 |
| License     | Boost 1.0 | Apache 2.0 | Apache 2.0 | Apache 2.0|
| Linkage     | header only | jar | jar | dll |
| Approach    | compile-time | run-time | compile-time (annotation processor) | run-time |
| Errors      | compile-time errors | exceptions | compile-time errors | exceptions |

| Library | Bind Interface |
| ------- | ---------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/di.cpp) |
| [Google.Guice] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/guice.java) |
| [Dagger2] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dagger2.java) |
| [Ninject] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/ninject.cs) |

| Library | Bind Value |
| ------- | ---------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/di.cpp) |
| [Google.Guice] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/guice.java) |
| [Dagger2] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dagger2.java) |
| [Ninject] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/ninject.cs) |

| Library | Missing Bindings Error |
| ------- | ---------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/di.cpp) |
| [Google.Guice] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/guice.java) |
| [Dagger2] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/dagger2.java) |
| [Ninject] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/ninject.cs) |

---

| Test | Types | Interfaces | Modules | Instances |
| ---- | ----- | ---------- | ------- | --------- |
| [Create Simple](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/create_simple) | 101 | 0 | 1 | 699150338 |

| Clang-3.7 -O2/Java8/Mono4.2 | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| --------------------------- | -------- | -------------- | --------- | --------- |
| Compilation time            | 0.707s   | 0.625s         | 2.445s    | 0.140s    |
| Execution time              | 0.002s   | -              | -         | -         |

---

| Test              | Types | Interfaces | Modules | Instances |
| ----------------- | ----- | ---------- | ------- | --------- |
| [Create Complex](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/create_complex) | 211 | 100 | 1 | 5874638529236910091 |

| Clang-3.7 -O2/Java8/Mono4.2 | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| --------------------------- | -------- | -------------- | --------- | --------- |
| Compilation time            | 3.082s   | 0.925s         | 4.283s    | 0.164s    |
| Execution time              | 0.002s   | -              | -         | -         |

* To run benchmarks
```sh
cd benchmark && make
```

---

[dicpp]: https://bitbucket.org/cheez/dicpp/wiki/Home
[Google.Fruit]: https://github.com/google/fruit
[Google.Guice]: https://github.com/google/guice
[Dagger2]: https://github.com/google/dagger
[Ninject]: https://github.com/ninject/ninject

[Bindings]: user_guide.md#bindings
[Scopes]: user_guide.md#scopes
[Providers]: user_guide.md#providers
[Policies]: user_guide.md#policies
[Config]: user_guide.md#di_config
[di::inject]: #di_inject
[di::ctor_traits]: #di_ctor_traits
[BOOST_DI_INJECT]: #BOOST_DI_INJECT
[BOOST_DI_INJECT_TRAITS]: #BOOST_DI_INJECT_TRAITS
