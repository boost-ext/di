[![Build Status](https://travis-ci.org/krzysztof-jusiak/di.svg?branch=cpp14)](https://travis-ci.org/krzysztof-jusiak/di)
[![Coverage Status](https://coveralls.io/repos/krzysztof-jusiak/di/badge.png?branch=cpp14)](https://coveralls.io/r/krzysztof-jusiak/di?branch=cpp14)
[![Stories in Ready](https://badge.waffle.io/krzysztof-jusiak/di.svg?label=ready&title=Ready)](http://waffle.io/krzysztof-jusiak/di)

### C++ Dependency Injection
> "Don't call us, we'll call you", Hollywood principle

Dependency injection is a programming practice providing required objects to an object.

* Provide loosely coupled code (separation of business logic and object creation)
* Provide easier to maintain code (different objects might be easily injected)
* Provide easier to test code (fakes objects might be injected)

```cpp
No Dependency injection                  Dependency Injection
----------------------------------------|--------------------------------------------
class example {                         | class example {
public:                                 | public:
    example()                           |     example(shared_ptr<logic> logic
        : logic_(new logic())           |           , shared_ptr<logger> logger)
        , logger_(factory::create())    |         : logic_(logic), logger_(logger)
    { }                                 |     { }
                                        |
    int run() const;                    |     int run() const;
                                        |
    shared_ptr<logic> logic_;           | private:
    shared_ptr<logger> logger_;         |     shared_ptr<logic> logic_;
};                                      |     shared_ptr<logger> logger_;
                                        | };
```

Boost.DI is a C++14 header only, type safe, compile time, non-intrusive constructor dependency injection
library improving manual dependency injection by simplifying object instantiation with automatic
dependencies injection.

* Reduce boilerplate code (no factories, no objects creation)
* Reduce cost of maintenance effort (constructor signature change won't affect di configuration)
* Reduce testing effort (automatic mocks Injector)
* Gives better control of what and how is created (policies, providers)
* Gives better understanding about objects hierarchy (Types creation graph)

```cpp
Manual Dependency Injection              Boost.DI
----------------------------------------|--------------------------------------------
int main() {                            | int main() {
    /*boilerplate code*/                |     auto injector = di::make_injector();
    auto logic = make_shared<logic>();  |     return injector.create<example>().run()
    auto logger = make_shared<logger>();| }
                                        |
    return example{logic, logger}.run();|
}                                       |
```

###How To Start

* [Documentation](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html)
* [Issues] (https://github.com/krzysztof-jusiak/di/issues?state=open)

Boost.DI requires C++14 compliant compiler(Clang-3.4+, GCC-5.0+) with STL (memory, new, type\_traits).
There are no Boost libraries requirements (optionally Boost.Smart\_Ptr).

> To get started the only file you need is `di.hpp`:

[https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/include/boost/di.hpp](https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/include/boost/di.hpp)

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
    cd di/build
    ./bootstrap.sh --with-toolset=clang
    ./b2 -j4 ../test ../example
```

**Quick User Guide**

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

*License*
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

*Disclaimers*
This is not an official Boost library yet and there is no guarantee it will ever be!

