###Quick Start

* Get [boost/di.hpp](https://raw.githubusercontent.com/boost-experimental/di/master/include/boost/di.hpp) header
```sh
    wget https://raw.githubusercontent.com/boost-experimental/di/master/include/boost/di.hpp
```

* Include the header and define `msm` namespace alias
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

###Configuration
| Macro                                   | Description |
| ----------------------------------------|------------ |
| `BOOST_DI_VERSION`                      | Current version of Boost.DI (ex. 1'0'0) |
| `BOOST_DI_CFG`                          | Global configuration allows to customize provider and policies |
| `BOOST_DI_CFG_DIAGNOSTICS_LEVEL`        | Global configuration allows to customize provider and policies |
| `BOOST_DI_CFG_CTOR_LIMIT_SIZE`          | Limits number of allowed consturctor parameters [0-10, default=10] |
| `BOOST_DI_NAMESPACE_BEGIN`              | Limits number of allowed consturctor parameters [0-10, default=10] |
| `BOOST_DI_NAMESPACE_END`                | Limits number of allowed consturctor parameters [0-10, default=10] |

###Other Libraries

###Performance

<a id="run_time_performance"></a>
> **[Run-time performance (-O2)](http://boost-experimental.github.io/di/cpp14/boost/libs/di/doc/html/di/performance.html)**
* Environment
    * x86\_64 Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz GenuineIntel GNU/Linux
    * clang++3.4 -O2 / `gdb -batch -ex 'file ./a.out' -ex 'disassemble main'`
```cpp
Create type wihtout bindings            | Asm x86-64 (same as `return 0`)
----------------------------------------|-----------------------------------------
int main() {                            | xor %eax,%eax
    auto injector = di::make_injector();| retq
    return injector.create<int>();      |
}                                       |
```
```cpp
Create type with bound instance         | Asm x86-64 (same as `return 42`)
----------------------------------------|-----------------------------------------
int main() {                            | mov $0x2a,%eax
    auto injector = di::make_injector(  | retq
        di::bind<int>.to(42)            |
    );                                  |
                                        |
    return injector.create<int>();      |
}                                       |
```
```cpp
Create named type                       | Asm x86-64 (same as `return 42`)
----------------------------------------|-----------------------------------------
auto my_int = []{};                     | mov $0x2a,%eax
                                        | retq
struct c {                              |
    BOOST_DI_INJECT(c                   |
        , (named = my_int) int i)       |
        : i(i)                          |
    { }                                 |
                                        |
    int i = 0;                          |
};                                      |
                                        |
int main() {                            |
  auto injector = di::make_injector(    |
    di::bind<int>.named(my_int).to(42)  |
  );                                    |
                                        |
  return injector.create<c>().i;        |
}                                       |
```
```cpp
Create bound interface                  | Asm x86-64 (same as `make_unique`)
----------------------------------------|-----------------------------------------
int main() {                            | push   %rax
    auto injector = di::make_injector(  | mov    $0x8,%edi
        di::bind<i1>.to<impl1>()        | callq  0x4007b0 <_Znwm@plt>
    );                                  | movq   $0x400a30,(%rax)
                                        | mov    $0x8,%esi
    auto ptr = injector.create<         | mov    %rax,%rdi
        unique_ptr<i1>                  | callq  0x400960 <_ZdlPvm>
    >();                                | mov    $0x1,%eax
                                        | pop    %rdx
    return ptr.get() != nullptr;        | retq
}                                       |
```
```cpp
Create bound interface via module       | Asm x86-64 (same as `make_unique`)
----------------------------------------|-----------------------------------------
struct module {                         | push   %rax
	auto configure() const noexcept {   | mov    $0x8,%edi
		return di::make_injector(       | callq  0x4007b0 <_Znwm@plt>
			di::bind<i1>.to<impl1>()    | movq   $0x400a10,(%rax)
		);                              | mov    $0x8,%esi
	}                                   | mov    %rax,%rdi
};                                      | callq  0x400960 <_ZdlPvm>
                                        | mov    $0x1,%eax
int main() {                            | pop    %rdx
	auto injector = di::make_injector(  | retq
        module{}                        |
    );                                  |
                                        |
	auto ptr = injector.create<         |
        unique_ptr<i1>                  |
    >();                                |
                                        |
	return ptr != nullptr;              |
}                                       |
```
```cpp
Create bound interface via exposed      | Asm x86-64
module                                  | cost = type erasure
----------------------------------------|----------------------------------------------------------------------
struct module {                         | push   %r14
	di::injector<i1> configure() const {| push   %rbx
		return di::make_injector(       | push   %rax
			di::bind<i1>.to<impl1>()    | mov    %rdi,%r14
		);                              | mov    $0x20,%edi
	}                                   | callq  0x400aa0 <_Znwm@plt>
};                                      | mov    %rax,%rbx
                                        | mov    $0x400ea0,%eax
int main() {                            | movq   %rax,%xmm0
	auto injector = di::make_injector(  | movups %xmm0,(%rbx)
        module{}                        | movq   $0x400ec0,0x10(%rbx)
    );                                  | mov    $0x8,%edi
                                        | callq  0x400aa0 <_Znwm@plt>
	auto ptr = injector.create<         | movq   $0x400fc8,(%rax)
        unique_ptr<i1>                  | mov    %rax,(%r14)
    >();                                | mov    0x10(%rbx),%rax
                                        | mov    %rbx,%rdi
	return ptr != nullptr;              | callq  *%rax
}                                       | mov    %rbx,%rdi
                                        | callq  0x400a10 <_ZdlPv@plt>
                                        | mov    %r14,%rax
                                        | add    $0x8,%rsp
                                        | pop    %rbx
                                        | pop    %r14
                                        | retq
                                        | mov    %rax,%rdi
										| callq  0x400e90 <__clang_call_terminate>
										| mov    %rax,%rdi
										| callq  0x400e90 <__clang_call_terminate>
										| mov    %rax,%rdi
										| callq  0x400e90 <__clang_call_terminate>
```

*

<a id="compile_time_performance"></a>
> **[Compile-time performance](http://boost-experimental.github.io/di/cpp14/boost/libs/di/doc/html/di/performance.html)** | [Example](https://github.com/boost-experimental/di/blob/cpp14/test/pt/di.cpp)
* Environment
    * x86\_64 Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz GenuineIntel GNU/Linux
    * clang++3.4 -O2
```cpp
Boost.DI header                         | Time [s]
----------------------------------------|-----------------------------------------
#include <boost/di.hpp>                 | 0.050
int main() { }                          |
```
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

<a id="diagnostic_messages"></a>
> **[Diagnostic messages](http://boost-experimental.github.io/di/cpp14/boost/libs/di/doc/html/di/diagnostic_messages.html)**
```cpp
Create interface without bound          | Error message
implementation                          |
----------------------------------------|[clang]--------------------------------------
auto injector = di::make_injector();    | warning: 'create' is deprecated: creatable constraint not satisfied
injector.create<i*>();                  |     injector.create<i*>();
                                        |              ^
                                        | note: 'create<i *, 0>' has been explicitly marked deprecated here
                                        |     T create() const {
                                        |       ^
                                        | error: inline function 'boost::di::abstract_type<i>::is_not_bound::error' is not defined
                                        |     error(_ = "type not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?")
                                        |     ^
                                        | note: used here
                                        |     constraint_not_satisfied{}.error();
                                        |
                                        |[gcc]----------------------------------------
                                        | error: inline function ‘constexpr T* boost::di::abstract_type<T>::is_not_bound::error(boost::di::_) const [with T = i]’ used but never defined
                                        |      error(_ = "type not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?")
                                        |      ^
                                        | error: call to ‘boost::di::core::injector<boost::di::config>::create<i*, 0>’ declared with attribute error: creatable constraint not satisfied
                                        |      injector.create<i*>();
```
```cpp
Ambiguous binding                       | Error message
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | error: base class 'pair<int, no_name>'
    di::bind<int>.to(42)                | specified more than once as a direct
  , di::bind<int>.to(87)                | base class
);                                      |
                                        |
injector.create<int>();                 |
```
```cpp
Create not bound object with all bound  | Error message
policy                                  |
----------------------------------------|-----------------------------------------
class all_bound : public di::config {   | error: static_assert failed
public:                                 | "Type T is not allowed"
  auto policies() const noexcept {      |
    return di::make_policies(           |
      constructible(is_bound<_>{})      |
    );                                  |
  }                                     |
};                                      |
                                        |
auto injector =                         |
    di::make_injector<all_bound>();     |
                                        |
injector.create<int>();                 |
```
```cpp
Wrong annotation                        | Error message
(NAMED instead of named)                |
----------------------------------------|-----------------------------------------
auto name = []{};                       | error: use of undeclared identifier
                                        | 'named'
struct c {                              |
    BOOST_DI_INJECT(c                   |
        , (NAMED = name) int) { }       |
};                                      |
                                        |
di::make_injector().create<c>();        |
```

*


***Not configurable***
![CPP](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/errors/not_configurable.cpp)
