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
* [MSVC-2015+](https://ci.appveyor.com/project/boost-experimental/di)

###Configuration
| Macro                                       | Description |
| --------------------------------------------|------------ |
| `BOOST_DI_VERSION`                          | Current version of Boost.DI (ex. 1'0'0) |
| [`BOOST_DI_CFG`](user_guide.md#di_config)   | Global configuration allows to customize provider and policies |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL`            | Gives more information with error messages |
| `BOOST_DI_CFG_CTOR_LIMIT_SIZE`              | Limits number of allowed consturctor parameters [0-10, default=10] |
| `BOOST_DI_NAMESPACE_BEGIN`                  | `namespace boost { namespace di { inline namespace v_1_0_0 {` |
| `BOOST_DI_NAMESPACE_END`                    | `}}}` |

###Exception Safety

* Boost.DI is not using exceptions internally and therefore might be compiled with `-fno-exceptions`.
* Check [User Guide](user_guide.md) to verify which API's are marked `noexcept`.

###Thread Safety

* Boost.DI is thread safe.

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

###Dependency Injection Libraries

* C++ Dependency Injection Libraries

|         | Boost.DI | [dicpp] | [Google.Fruit] |
| ------- | -------- | ----- | ------------ |
| Version | 1.0.0 | HEAD | 1.0.2 |
| Language | C++14 | C++11 | C++11 |
| License | Boost 1.0 | Boost 1.0 | Apache 2.0|
| Linkage | header only | header only | library |
| Approach | compile-time | run-time | compile/run-time |
| Errors | compile-time errors | exceptions | compile-time errors + exceptions |

| Library | Bind Interface |
| ------- | ---------- |
| Boost.DI | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/di.cpp) |
| [dicpp] | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dicpp.cpp) |
| [Google.Fruit] | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/fruit.cpp) |

| Library | Bind Value |
| ------- | ---------- |
| Boost.DI | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/di.cpp) |
| [dicpp] | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dicpp.cpp) |
| [Google.Fruit] | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/fruit.cpp) |

| Library | Missing Binding Error |
| ------- | ---------- |
| Boost.DI | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/di.cpp) |
| [dicpp] | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/dicpp.cpp) |
| [Google.Fruit] | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/fruit.cpp) |

---

| Test | CXXFLAGS | Types | Instances created | Modules |
| ---- | -------- | ----- | ----------------- | ------- |
|[Create Simple]() | -O2 -s | 0 | 0 | 0 | 0 |

| Create Simple                   | Boost.DI | [dicpp] | [Google.Fruit] |
| ------------------------------- | -------- | ------- | -------------- |
| Compilation time                | 0.000s   | 0.000s  | 0.000s (-DFRUIT_NO_LOOP) |
| Execution time                  | 0ms      | 0ms     | 0ms    |
| Memory usage / sizeof(injector) | 0b       | 0b      | 0b     |
| Executable size                 | 0kb      | 0kb     | 0kb    |

---

| Test | CXXFLAGS | Types | Instances created | Modules |
| ---- | -------- | ----- | ----------------- | ------- |
|[Create Complex]() | -O2 -s | 0 | 0 | 0 | 0 |

| Create Complex                  | Boost.DI | [dicpp] | [Google.Fruit] |
| ------------------------------- | -------- | ------- | -------------- |
| Compilation time                | 0.000s   | 0.000s  | 0.000s (-DFRUIT_NO_LOOP) |
| Execution time                  | 0ms      | 0ms     | 0ms    |
| Memory usage / sizeof(injector) | 0b       | 0b      | 0b     |
| Executable size                 | 0kb      | 0kb     | 0kb    |

---

* Dependency Injection Libraries

|         | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| ------- | -------- | ------------ | ------- | ------- |
| Language | C++14 | Java 6 | Java | C# |
| License | Boost 1.0 | Apache 2.0 | Apache 2.0 | Apache 2.0|
| Linkage | header only | jar | jar | dll |
| Approach | compile-time | run-time | compile-time (annotation processor) | run-time |
| Errors | compile-time errors | exceptions | compile-time errors | exceptions |

| Library | Bind Interface |
| ------- | ---------- |
| Boost.DI | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/di.cpp) |
| [Google.Guice] | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/guice.cpp) |
| [Dagger2] | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dagger2.cpp) |
| [Ninject] | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/Ninject.cpp) |

| Library | Bind Value |
| ------- | ---------- |
| Boost.DI | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/di.cpp) |
| [Google.Guice] | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/guice.cpp) |
| [Dagger2] | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dagger2.cpp) |
| [Ninject] | ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/Ninject.cpp) |

| Library | Missing Bindings Error |
| ------- | ---------- |
| Boost.DI | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/di.cpp) |
| [Google.Guice] | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/guice.cpp) |
| [Dagger2] | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/dagger2.cpp) |
| [Ninject] | ![CPP(RESULT)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/Ninject.cpp) |

---

| Test | CXXFLAGS | Types | Instances created | Modules |
| ---- | -------- | ----- | ----------------- | ------- |
|[Create Simple]() | -O2 -s | 0 | 0 | 0 | 0 |

| Create Simple                   | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| ------------------------------- | -------- | -------------- | --------- | --------- |
| Compilation time                | 0.000s   | 0.000s         | 0.000s    | 0.000s    |
| Execution time                  | 0ms      | 0ms            | 0ms       | 0ms       |

---

| Test | CXXFLAGS | Types | Instances created | Modules |
| ---- | -------- | ----- | ----------------- | ------- |
|[Create Complex]() | -O2 -s | 0 | 0 | 0 | 0 |

| Create Complex                  | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| ------------------------------- | -------- | -------------- | --------- | --------- |
| Compilation time                | 0.000s   | 0.000s         | 0.000s    | 0.000s    |
| Execution time                  | 0ms      | 0ms            | 0ms       | 0ms       |

---

[dicpp]: https://bitbucket.org/cheez/dicpp/wiki/Home
[Google.Fruit]: https://github.com/google/fruit
[Google.Guice]: https://github.com/google/guice
[Dagger2]: https://github.com/google/dagger
[Ninject]: https://github.com/ninject/ninject
