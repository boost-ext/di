###Quick Start

* Get [boost/di.hpp](https://raw.githubusercontent.com/boost-experimental/di/master/include/boost/di.hpp) header
```sh
wget https://raw.githubusercontent.com/boost-experimental/di/master/include/boost/di.hpp
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
git clone https://github.com/boost-experimental/di && cd di && make test
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
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL`  | Gives more information with error messages (See [Error messages](#error-messages)) |
| `BOOST_DI_CFG_CTOR_LIMIT_SIZE`    | Limits number of allowed consturctor parameters [0-10, default=10] (See [Injections](user_guide.md#injections)) |
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
| [Scopes] | Responsible for maintain objects life time |
| [Providers] | Responsible for providing object instance |
| [Policies] | Compile-time limitations for types / Run-time types vistor |
| [Config] | Configuration for [Policies] and [Providers] |
| Core | Responsible for resolving requested types (implementation detail) |
| Wrappers | Responsible for conversion to required type (implementation detail) |

* In a nutshell

```cpp
template<class TConfig, class... TBindings>
struct core::injector : TBindings... {
  template<class T>
  auto create() const noexcept {
    TConfig::policies<T>()...;
    auto dependency = core::binder<TBindings...>().resolve<T>(this);
    return core::wrapper<T>{dependency.create(TConfig::provider().get<T>())}; // get<T> -> create<ctor_traits<T>...>()
  }
};
```

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
    ctor    = raw constructor: c(int i, double d);
    inject  = inject constructor: BOOST_DI_INJECT(c, int i, double d);
    all     = all types exposed from module: auto configure();
    exposed = one type exposed from module: di::injector<c> configure();
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

* C++ Dependency Injection Libraries

    * [https://bitbucket.org/cheez/dicpp](https://bitbucket.org/cheez/dicpp)
    * [https://github.com/google/fruit](https://github.com/google/fruit)
    * [https://github.com/phs/sauce](https://github.com/phs/sauce)
    * [http://wallaroolib.sourceforge.net](http://wallaroolib.sourceforge.net)
    * [https://github.com/ybainier/hypodermic](https://github.com/ybainier/hypodermic)
    * [https://code.google.com/p/infectorpp](https://code.google.com/p/infectorpp)
    * [https://github.com/vogel/injeqt](https://github.com/vogel/injeqt)
    * [https://github.com/gracicot/kangaru](https://github.com/gracicot/kangaru)
    * [https://github.com/admiyo/CppInject](https://github.com/admiyo/CppInject)
    * [http://codebros.github.io/DiLite](http://codebros.github.io/DiLite)

|         | Boost.DI | [dicpp] | [Google.Fruit] |
| ------- | -------- | ----- | ------------ |
| Version | 1.0.0 | HEAD | (pre) 1.1.0 |
| Language | C++14 | C++11 | C++11 |
| License | Boost 1.0 | Boost 1.0 | Apache 2.0|
| Linkage | header only | header only | library |
| Dependencies | - | boost system/boost thread | - |
| Approach | compile-time | run-time | compile/run-time |
| Errors | compile-time errors | exceptions | compile-time errors + exceptions |

| Library | Bind Interface | ASM x86-64 |
| ------- | -------------- | ---------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/di.cpp) | [8 instructions](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/di.cpp) |
| [dicpp] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dicpp.cpp) | [50 instructions](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dicpp.cpp) |
| [Google.Fruit] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/fruit.cpp) | [82 instructions](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dicpp.cpp) |

| Library | Bind Value | ASM x86-64 |
| ------- | ---------- | ---------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/di.cpp) | [8 instructions](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/di.cpp) |
| [dicpp] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dicpp.cpp) | [50 instructions](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dicpp.cpp) |
| [Google.Fruit] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/fruit.cpp) | [82 instructions](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dicpp.cpp) |

| Library | Missing Binding Error |
| ------- | ---------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/di.cpp) |
| [dicpp] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/dicpp.cpp) |
| [Google.Fruit] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/fruit.cpp) |

---

| Test | CXXFLAGS | Types | Interfaces | Modules | Instances |
| ---- | -------- | ----- | ---------- | ------- | --------- |
| [Create Simple](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/create_simple) | -O2 | 101 | 0 | 1 | 699150338 |

| Create Simple             | Boost.DI | [dicpp] | [Google.Fruit] |
| ------------------------- | -------- | ------- | -------------- |
| Compilation time          | 0.707s   | 8.226s  | 6.724s (-DFRUIT_NO_LOOP_CHECK) |
| Execution time            | 0.002s   | -       | -      |
| Memory usage (stack/heap) | 1b/0b    | -       | -      |
| Executable size           | 4.2K     | 119K    | 892kb  |

---

| Test              | CXXFLAGS | Types | Interfaces | Modules | Instances | 
| ----------------- | -------- | ----- | ---------- | ------- | --------- |
| [Create Complex](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/create_complex) | -O2      | 211   | 100        | 1       | 5874638529236910091 |

| Clang-3.7                 | Boost.DI | [dicpp]    | [Google.Fruit] |
| ------------------------- | -------- | ---------- | -------------- |
| Compilation time          | 3.082s   | 1m14.080s  | 34.461s (-DFRUIT_NO_LOOP_CHECK) |
| Execution time            | 0.002s   | -          | -    |
| Memory usage (stack/heap) | 1b/0b    | -          | -    |
| Executable size           | 72kb     | 4.4M       | 5.1M |

---

* Dependency Injection Libraries

|          | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| -------  | -------- | ------------ | ------- | ------- |
| Language | C++14    | Java 6 | Java | C# |
| Version  | 1.0.0    | 4.0 | 2.0.2 | 3.0.2 |
| License  | Boost 1.0 | Apache 2.0 | Apache 2.0 | Apache 2.0|
| Linkage  | header only | jar | jar | dll |
| Approach | compile-time | run-time | compile-time (annotation processor) | run-time |
| Errors   | compile-time errors | exceptions | compile-time errors | exceptions |

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

| Test | CXXFLAGS | Types | Instances created | Modules |
| ---- | -------- | ----- | ----------------- | ------- |
|[Create Simple](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/create_simple) | -O2 | 0 | 0 | 0 | 0 |

| Create Simple     | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| ----------------- | -------- | -------------- | --------- | --------- |
| Compilation time  | 0.707s   | 0.625s         | 2.445s    | 0.140s    |
| Execution time    | 0.002s   | -              | -         | -         |

---

| Test | CXXFLAGS | Types | Instances created | Modules |
| ---- | -------- | ----- | ----------------- | ------- |
|[Create Complex](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/create_complex) | -O2 | 0 | 0 | 0 | 0 |

| Create Complex    | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| ----------------- | -------- | -------------- | --------- | --------- |
| Compilation time  | 3.082s   | 0.925s         | 4.283s    | 0.164s    |
| Execution time    | 0.002s   | -              | -         | -         |

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
