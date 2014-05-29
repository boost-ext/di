# C++ Dependency Injection
> "Don't call us, we'll call you", Hollywood principle

## Disclaimers
This is not an official Boost library yet and there is no guarantee it will ever be.

## Introduction
DI is C++03/C++11 header only library providing compile time, macro free constructor dependency injection functionality.

## Hello World
```cpp
#include <boost/di.hpp>

namespace di = boost::di;

struct i { virtual ~i() { } };
struct impl : i { };
struct some_name { };

struct hello
{
    hello(const std::shared_ptr<i>& sp, double d)
        : sp(sp)
    {
        assert(dynamic_cast<impl*>(sp.get()));
        assert(d == 0.0); // default zero initialization
    }

    std::shared_ptr<i> sp;
};

struct world {
    world(hello copy
        , boost::shared_ptr<i> sp
        , int i
        , di::named<const std::string&, some_name> str)
    {
        std::string s = str;
        assert(dynamic_cast<impl*>(sp.get()));
        assert(copy.sp.get() == sp.get());
        assert(i == 42);
        assert(s == "some_name");
    }
};

int main() {
    auto injector = di::make_injector(
        di::bind_int<42>() // static value
      , di::bind<std::string>::named<some_name>::to("some_name") // external value
      , di::bind<i, impl>() // scope deduction -> di::shared<di::bind<i, impl>>
    );

    auto hello_world = injector.create<world>();

    return 0;
}
```

## Main Features
* Header only library
* Architecture independent (tested on x86/x86\_64)
* Supports all POSIX and Windows operating systems (tested on Linux 3.8/Windows 7/8)
* Supports C++03/C++11 standard
* Exception safe guaranty
* Thread safe guaranty
* Constructor injection (macro free)
* Compile time creation guaranty
* Compile time policies (arguments permission, binding correctness, circular dependencies, creation ownership, scopes permission)
* Dependencies life time management (scopes: deduce, external, unique, shared, session + custom scopes)
* Scope deduction (shared\_ptr -> shared, unique\_ptr, lvalue -> unique, ...)
* Supports copies, references, pointers, boost and std smart pointers / rvalue references(C++11)
* Named parameters (named\<int, my\_int\>)
* Runtime visitor throughout created objects (useful for generation dependency diagrams)

## More information
* [Documentation](http://krzysztof-jusiak.github.com/di/doc/html)
* [Todo/Issues] (https://github.com/krzysztof-jusiak/di/issues?state=open)

## Tested compilers
* Linux
   * Clang 3.2/3.3/3.4/3.4.1 (clean with Clang Static Analyzer and Valgrind)
   * GCC 4.7.3/4.8.2/4.9.0 (clean with Valgrind)
   * Intel C++ 14.0.3

* Windows
   * MinGW 4.7.3/4.8.2
   * Visual Studio 2013 (clean with DrMemory)

* Mac
   * Clang 3.4.1

## Continuous integration
[![Build Status](https://travis-ci.org/krzysztof-jusiak/di.png?branch=master)](https://travis-ci.org/krzysztof-jusiak/di)
[![Build Status](https://ci.appveyor.com/api/projects/status/1il4knxh7tq9o5ic)](https://ci.appveyor.com/project/krzysztof-jusiak/di)
[![Coverage Status](https://coveralls.io/repos/krzysztof-jusiak/di/badge.png?branch=master)](https://coveralls.io/r/krzysztof-jusiak/di?branch=master)

## License
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

