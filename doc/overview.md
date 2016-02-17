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

###Supported/tested compilers

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

| Expression | Description | Error | Suggestion |
| ---------- | ----------- | ----- | ---------- |
| ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/boundable_type_is_abstract.cpp) | type `T` is abstract | type<T>::is_abstract | |
| ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/boundable_type_is_bound_more_than_once.cpp) | type `T` is bound more than once | type<T>::is_bound_more_than_once | |
| ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/creatable_abstract_type_is_not_bound.cpp) | | abstract_type<interface\>::is_not_bound | 'type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?' |
| ![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/errors/creatable_expose_abstract_type_is_not_bound.cpp) | | | |

###Dependency Injection Libraries

* C++ Dependency Injection Libraries

| Library | Boost.DI | dicpp | Google Fruit |
| ------- | -------- | ----- | ------------ |
| Language | C++14 | C++11 | C++11 |
| License | Boost 1.0 | Boost 1.0 | Apache 2.0|
| Linkage | header only | header only | library |
| Approach | compile-time | run-time | compile/run-time |

* Dependency Injection Libraries

| Library | Boost.DI | Google Guice | Dagger2 | Ninject |
| ------- | -------- | ------------ | ------- | ------- |
| Language | C++14 | Java 6 | Java | C# |
| License | Boost 1.0 | Apache 2.0 | Apache 2.0 | Apache 2.0|
| Linkage | header only | jar | jar | dll |
| Approach | compile-time | run-time | compile-time (annotation processor) | run-time |
