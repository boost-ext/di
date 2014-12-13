### Versions
> [C++03/11](https://github.com/krzysztof-jusiak/di/tree/cpp03) | **C++14**

### C++ Dependency Injection
> "Don't call us, we'll call you", Hollywood principle

### Disclaimers
This is not an official Boost library yet and there is no guarantee it will ever be!

### Introduction
Boost.DI is C++14 header only library providing type safe, compile time, macro free constructor dependency injection.

* [Documentation](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html)
* [Todo/Issues] (https://github.com/krzysztof-jusiak/di/issues?state=open)
* [Boost Library Incubator](http://rrsd.com/blincubator.com/bi_library/di-dependency-injection-2)
* [Boost Library Proposal](http://boost.2283326.n4.nabble.com/Is-there-any-interest-in-a-dependency-injection-library-td4665526.html)

### Instalation
> Boost.DI is header only library which requires C++14 compliant compiler(Clang-3.4+, GCC-5.0+) with STL (memory, new, type\_traits). There are no Boost libraries requirements (optically Boost.Smart\_Ptr).

> To get started the only file you need is `di.hpp`:

[https://github.com/krzysztof-jusiak/di/blob/cpp14/include/boost/di.hpp](https://github.com/krzysztof-jusiak/di/blob/cpp14/include/boost/di.hpp)

```cpp
    // main.cpp
    #include "di.hpp"
    int main() { }
```

```sh
    clang++ -std=c++1y main.cpp
```

> To get and test Boost.DI library:

```sh
    git clone --recursive https://github.com/krzysztof-jusiak/di.git
    cd build
    ./bootstrap.sh --with-toolset=clang
    ./b2 -j4 ../test ../example
```

**Hello World**

```cpp
#include <memory>
#include <boost/di.hpp>

namespace di = boost::di;

class hello_world {
public:
    hello_world(std::shared_ptr<ilogger> logger
              , std::shared_ptr<ilogic> logic
              , bool value)
        : logger_(logger)
        , logic_(logic)
        , value_(value)
    { }

    int run() const {
        if (value_) {
            logger_->log("hello world");
            return logic_->do_it();
        }
    }

    std::shared_ptr<ilogger> logger_;
    std::shared_ptr<ilogic> logic_;
    bool value_ = false;
};

int main() {
    // dependency injection configuration
    auto injector = di::make_injector(
        di::bind<ilogger, logger>
      , di::bind<ilogic, logic>
      , di::bind<bool>.to(true)
    );

    // composition root
    return injector.create<hello_world>().run();
}
```

**Tests with [Automatic Mocks Injector](https://github.com/krzysztof-jusiak/mocks_injector)**

```cpp
#include <mocks_injector.hpp>

void test_hello_world_when_run_with_value_false_then_nothing() {
    auto mi = di::make_mocks_injector(di::bind<bool>::to(false));
    mi.create<hello_world>()->run();
}

void test_hello_world_when_run_with_value_true_then_log_and_do_it() {
    auto mi = di::make_mocks_injector(di::bind<bool>::to(true));

    EXPECT_CALL(mi, ilogic::do_it);
    EXPECT_CALL(mi, ilogger::log).With("hello world");

    mi.create<hello_world>()->run();
}
```

### Features
* Type safe
* Header only
* Minimal runtime overhead
* Really quick compilation times
* Requires C++14 standard compliant compiler (STL headers: memory/new/type\_traits)
* No Boost libraries requirements (optional requirement to support Boost Smart Pointers)
* Easily extendible (custom scopes/providers including allocation strategy)
* Exception safety strong guaranty
* Thread safe guaranty
* Compile time creation guaranty - no exceptions - if application compiles all dependencies will be be created accurately
* Macro free - by default no need to specify constructor traits or register anything (less intrusive)
* Scopes deduction - scopes are deduced based on type semantic
* Automatic conversion between std/boost smart pointers
* Dependencies life time management (scopes: deduce, external, unique, shared, session + custom scopes)
* Compile time policies - ex. to limit supported types only to specified
* Supports copies, references, pointers, boost and std smart pointers / rvalue references(C++11)
* Supports annotations (di::named\<int, my\_int\>)
* Runtime visitor throughout created objects (useful for generation dependency diagrams)

### Tested compilers (100% test code coverage)
* Linux (x86/x86-64)
   * Clang 3.4/3.5/3.6+ (clean with Clang Static Analyzer and Valgrind)
   * GCC 5.0+ (clean with Valgrind)

* Windows (x86/x86-64)
   * MinGW 5.0+

* Darwin/Mac OS (x86-64)
   * Clang 503+

### Continuous integration
[![Build Status](https://travis-ci.org/krzysztof-jusiak/di.png?branch=master)](https://travis-ci.org/krzysztof-jusiak/di)
[![Build Status](https://ci.appveyor.com/api/projects/status/1il4knxh7tq9o5ic)](https://ci.appveyor.com/project/krzysztof-jusiak/di)
[![Coverage Status](https://coveralls.io/repos/krzysztof-jusiak/di/badge.png?branch=master)](https://coveralls.io/r/krzysztof-jusiak/di?branch=master)

### Status
[![Stories in Ready](https://badge.waffle.io/krzysztof-jusiak/di.svg?label=ready&title=Ready)](http://waffle.io/krzysztof-jusiak/di)

### Other Dependency Injection Frameworks for C++
 * [https://bitbucket.org/cheez/dicpp](https://bitbucket.org/cheez/dicpp)
 * [https://github.com/google/fruit](https://github.com/google/fruit)
 * [https://code.google.com/p/infectorpp](https://code.google.com/p/infectorpp)
 * [https://github.com/phs/sauce](https://github.com/phs/sauce)
 * [https://code.google.com/p/wallaroo](https://code.google.com/p/wallaroo)
 * [https://code.google.com/p/hypodermic](https://code.google.com/p/hypodermic)
 * [https://github.com/admiyo/CppInject](https://github.com/admiyo/CppInject)
 * [http://qtioccontainer.sourceforge.net](http://qtioccontainer.sourceforge.net)
 * [https://code.google.com/p/autumnframework](https://code.google.com/p/autumnframework)
 * [http://sourceforge.net/projects/cpp-resolver](http://sourceforge.net/projects/cpp-resolver)
 * [https://code.google.com/p/pococapsule](https://code.google.com/p/pococapsule)
 * [https://code.google.com/p/spring-cpp](https://code.google.com/p/spring-cpp)
 * [http://www.cs.utah.edu/~arichard/inject.html](http://www.cs.utah.edu/~arichard/inject.html)
 * [http://bobah.net/d4d/source-code/misc/cpp-dependency-injection](http://bobah.net/d4d/source-code/misc/cpp-dependency-injection)
 * [https://code.google.com/p/ffead-cpp](https://code.google.com/p/ffead-cpp)
 * [http://codebros.github.io/DiLite](http://codebros.github.io/DiLite)
 * [https://code.google.com/p/kindi](https://code.google.com/p/kindi)
 * [https://www.leapmotion.com/blog/hello-world-meet-autowiring-c](https://www.leapmotion.com/blog/hello-world-meet-autowiring-c)

### License
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

