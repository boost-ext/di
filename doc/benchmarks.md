##C++ Libraries

| **Library** | Boost.DI ([See Performance](overview.md#performance)) | [dicpp] | [Google.Fruit] |
| ----------- | -------- | ----- | ------------ |
| Version     | 1.0.0 | HEAD | 2.0.2 |
| Language    | C++14 | C++11 | C++11 |
| License     | Boost 1.0 | Boost 1.0 | Apache 2.0|
| Linkage     | header only | header only | library |
| Dependencies| - | boost system/boost thread | - |
| Approach    | compile-time | run-time | compile/run-time |
| Errors      | compile-time errors | exceptions | compile-time errors/exceptions |

* Environment
    * x86\_64 Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz GenuineIntel GNU/Linux
    * $CXX -O2

| Create a unique objects tree for each iteration<br /><br />`TYPES: 64`  `BINDINGS: 0`  `ITERATIONS: 1024` | 
|  ------------------------ | ------------ | -------- | ---------- | -------------- |
| **Clang-3.7**             | **Baseline** | **Boost.DI** | **[dicpp]**    | **[Google.Fruit]**              |
| Compilation time          |              | 3.082s       | 1m14.080s      | 34.461s / FRUIT_NO_LOOP_CHECK |
| Execution time            |              | 0.002s       | -              | -                               |
| Memory usage (stack/heap) |              | 1b/0b        | -              | -                               |
| Executable size           |              | 72kb         | 4.4M           | 5.1M                            |
| |  |  |  |
| **GCC-5.2** |  |  |  |
| Compilation time          |              | 3.082s       | 1m14.080s      | 34.461s / FRUIT_NO_LOOP_CHECK |
| Execution time            |              | 0.002s       | -              | -                               |
| Memory usage (stack/heap) |              | 1b/0b        | -              | -                               |
| Executable size           |              | 72kb         | 4.4M           | 5.1M                            |

* To run benchmarks
```sh
cd benchmark && make
```
---

##C++ vs Java vs C# Libraries

| **Library** | Boost.DI | [Google.Guice] | [Dagger2] | [Ninject] |
| ----------  | -------- | ------------ | ------- | ------- |
| Language    | C++14    | Java | Java | C# |
| Version     | 1.0.0    | 4.0 | 2.4 | 3.2 |
| License     | Boost 1.0 | Apache 2.0 | Apache 2.0 | Apache 2.0|
| Linkage     | header only | jar | jar | dll |
| Approach    | compile-time | run-time | compile-time (annotation processor) | run-time |
| Errors      | compile-time errors | exceptions | compile-time errors | exceptions |

* **Environment**
    * x86\_64 Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz GenuineIntel GNU/Linux
    * $CXX -O2/Java8/Mono4.2

>

* **Baseline**
    * Manual object creation

| Create a unique objects tree for each iteration<br /><br />`TYPES: 64`  `BINDINGS: 0`  `ITERATIONS: 1024` | 
| ----------------------------------- | ------------ | ------------ | ------------------------------ | ------------------ | ------------- | --------------- |
| **Clang-3.7 -O2 / Java8 / Mono4.2** | **Baseline** | **Boost.DI** | **[Google.Fruit]**             | **[Google.Guice]** | **[Dagger2]** | **[Ninject]**   |
| Compilation time                    | 0.063s       | 0.376s       | 2.329s / FRUIT_NO_LOOP_CHECK   | 0.570s             | 1.411s        | 0.144s + 0.079s |
| Executable size                     | 4.2K         | 8.5K         | 213K                           | -                  | -             | -               |
| Execution time                      | 0.002s       | 0.002s       | 0.037s                         | 0.528s             | 0.157s        | 1.131s          |

| Create a unique objects tree for each iteration<br /><br />`TYPES: 128`  `BINDINGS: 0`  `ITERATIONS: 1024` | 
| ----------------------------------- | ------------ | ------------ | ------------------------------ | ------------------ | ------------- | --------------- |
| **Clang-3.7 -O2 / Java8 / Mono4.2** | **Baseline** | **Boost.DI** | **[Google.Fruit]**             | **[Google.Guice]** | **[Dagger2]** | **[Ninject]**   |
| Compilation time                    | 0.082s       | 0.706s       | 4.375s / FRUIT_NO_LOOP_CHECK   | 0.642s             | 1.903s        | 0.149s + 0.093s |
| Executable size                     | 4.2K         | 8.5K         | 513K                           | -                  | -             | -               |
| Execution time                      | 0.002s       | 0.002s       | 0.074s                         | 0.544s             | 0.210s        | 2.230s          |

| Create a unique objects tree for each iteration<br /><br />`TYPES: 256`  `BINDINGS: 0`  `ITERATIONS: 1024` | 
| ----------------------------------- | ------------ | ------------ | ------------------------------ | ------------------ | ------------- | --------------- |
| **Clang-3.7 -O2 / Java8 / Mono4.2** | **Baseline** | **Boost.DI** | **[Google.Fruit]**             | **[Google.Guice]** | **[Dagger2]** | **[Ninject]**   |
| Compilation time                    | 0.131s       | 1.328s       | 9.641s / FRUIT_NO_LOOP_CHECK   | 0.783s             | 2.814s        | 0.151s + 0.114s |
| Executable size                     | 4.2K         | 8.7K         | 1.4M                           | -                  | -             | -               |
| Execution time                      | 0.003s       | 0.003s       | 0.154s                         | 0.723s             | 0.323s        | 4.838s          |

| Create a unique objects tree for each iteration<br /><br />`TYPES: 512`  `BINDINGS: 0`  `ITERATIONS: 1024` | 
| ----------------------------------- | ------------ | ------------ | ------------------------------ | ------------------ | ------------- | --------------- |
| **Clang-3.7 -O2 / Java8 / Mono4.2** | **Baseline** | **Boost.DI** | **[Google.Fruit]**             | **[Google.Guice]** | **[Dagger2]** | **[Ninject]**   |
| Compilation time                    | 0.215s       | 2.459s       | 23.924s / FRUIT_NO_LOOP_CHECK  | 1.054s             | 4.231s        | 0.157s + 0.161  |
| Executable size                     | 8.2K         | 13K          | 4.2M                           | -                  | -             | -               |
| Execution time                      | 0.003s       | 0.003s       | 0.328s                         | 0.943s             | 0.547s        | 11.123s         |

| Create a unique objects tree for each iteration<br /><br />`TYPES: 1024`  `BINDINGS: 0`  `ITERATIONS: 1024` | 
| ----------------------------------- | ------------ | ------------ | ------------------------------ | ------------------ | ------------- | --------------- |
| **Clang-3.7 -O2 / Java8 / Mono4.2** | **Baseline** | **Boost.DI** | **[Google.Fruit]**             | **[Google.Guice]** | **[Dagger2]** | **[Ninject]**   |
| Compilation time                    | 0.402s       | 4.911s       | 1m9.601s / FRUIT_NO_LOOP_CHECK | 1.357s             | 5.943s        | 0.167s + 0.258  |
| Executable size                     | 12K          | 17K          | 15M                            | -                  | -             | -               |
| Execution time                      | 0.004s       | 0.004s       | 0.765s                         | 1.334s             | 0.998s        | 21.808s         |

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


