##C++ Libraries

| **Library** | Boost.DI ([See Performance](overview.md#performance)) | [dicpp] | [Google.Fruit] |
| ----------- | -------- | ----- | ------------ |
| Version     | 1.0.0 | HEAD | (pre) 1.1.0 |
| Language    | C++14 | C++11 | C++11 |
| License     | Boost 1.0 | Boost 1.0 | Apache 2.0|
| Linkage     | header only | header only | library |
| Dependencies| - | boost system/boost thread | - |
| Approach    | compile-time | run-time | compile/run-time |
| Errors      | compile-time errors | exceptions | compile-time errors/exceptions |

* Environment
    * x86\_64 Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz GenuineIntel GNU/Linux
    * $CXX -O2

| Create a unique objects tree for each iteration<br /><br />`TYPES: 64`  `BINDINGS: 0`  `ITERATIONS: 1000000` | 
|  ------------------------- | -------- | ---------- | -------------- |
| **Clang-3.7** | **Boost.DI** | **[dicpp]**    | **[Google.Fruit]** |
| Compilation time          | 3.082s   | 1m14.080s  | 34.461s (-DFRUIT_NO_LOOP_CHECK) |
| Execution time            | 0.002s   | -          | -    |
| Memory usage (stack/heap) | 1b/0b    | -          | -    |
| Executable size           | 72kb     | 4.4M       | 5.1M |
| |  |  |  |
| **GCC-5.2** |  |  |  |
| Compilation time          | 3.082s   | 1m14.080s  | 34.461s (-DFRUIT_NO_LOOP_CHECK) |
| Execution time            | 0.002s   | -          | -    |
| Memory usage (stack/heap) | 1b/0b    | -          | -    |
| Executable size           | 72kb     | 4.4M       | 5.1M |

* To run benchmarks
```sh
cd benchmark && make
```
---

##C++ vs Java vs C# Libraries

| **Library** | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| ----------  | -------- | ------------ | ------- | ------- |
| Language    | C++14    | Java | Java | C# |
| Version     | 1.0.0    | 4.0 | 2.0.2 | 3.0.2 |
| License     | Boost 1.0 | Apache 2.0 | Apache 2.0 | Apache 2.0|
| Linkage     | header only | jar | jar | dll |
| Approach    | compile-time | run-time | compile-time (annotation processor) | run-time |
| Errors      | compile-time errors | exceptions | compile-time errors | exceptions |

* Environment
    * x86\_64 Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz GenuineIntel GNU/Linux
    * $CXX -O2/Java8/Mono4.2

| Create a unique objects tree for each iteration<br /><br />`TYPES: 64`  `BINDINGS: 0`  `ITERATIONS: 1000000` | 
| -------------------- | --------------------------- | -------- | -------------- | --------- |
| **Clang-3.7 -O2/Java8/Mono4.2** | **Boost.DI** | **[Google.Guice]** | **[Dagger2]** | **[Ninject]** |
| Compilation time            | 0.707s   | 0.625s         | 2.445s    | 0.140s    |
| Execution time              | 0.002s   | -              | -         | -         |

* To run benchmarks
```sh
cd benchmark && make
```
---

##Usage of C++ vs Java vs C# Libraries

| Library | Bind Interface | Performance |
| ------- | -------------- | ----------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/di.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/di.cpp) |
| [dicpp] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dicpp.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dicpp.cpp) |
| [Google.Fruit] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/fruit.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/fruit.cpp) |
| [Google.Guice] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/guice.java) |
| [Dagger2] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/dagger2.java) |
| [Ninject] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_interface/ninject.cs) |

| Library | Bind Value | Performance |
| ------- | ---------- | ----------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/di.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/di.cpp) |
| [dicpp] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dicpp.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dicpp.cpp) |
| [Google.Fruit] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/fruit.cpp) | [ASM x86-64](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/fruit.cpp) |
| [Google.Guice] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/guice.java) |
| [Dagger2] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/dagger2.java) |
| [Ninject] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_value/ninject.cs) |

| Library | Missing Bindings Error |
| ------- | ---------- |
| Boost.DI | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/di.cpp) |
| [dicpp] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/dicpp.cpp) |
| [Google.Fruit] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/fruit.cpp) |
| [Google.Guice] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/guice.java) |
| [Dagger2] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/dagger2.java) |
| [Ninject] | ![CPP(SHOW)](https://raw.githubusercontent.com/boost-experimental/di/cpp14/benchmark/bind_error/ninject.cs) |

[dicpp]: https://bitbucket.org/cheez/dicpp/wiki/Home
[Google.Fruit]: https://github.com/google/fruit
[Google.Guice]: https://github.com/google/guice
[Dagger2]: https://github.com/google/dagger
[Ninject]: https://github.com/ninject/ninject


