**C++ Dependency Injection**

### Disclaimers
This is not an official Boost library yet and there is no guarantee it will ever be.

## Introduction
DI is C++03/C++11, header only library providing compile time, macro free constructor dependency injection functionality.

### Hello World
```cpp
#include <boost/di.hpp>

namespace di = boost::di;

struct i { virtual ~i() { } };
struct impl : i { };

struct hello_world {
    hello_world(std::shared_ptr<i> p1, boost::shared_ptr<i> p2, int i, const std::string& txt) {
        assert(dynamic_cast<impl*>(p1.get()));
        assert(dynamic_cast<impl*>(p2.get()));
        assert(p1.get() == p2.get());
        assert(i == 42);
        assert(txt == "text");
    }
};

int main() {
    auto injector = di::make_injector(
        di::bind_int<42>() // static
      , di::bind<std::string>::to("text") // external
      , di::bind<i, impl>() // di::shared<di::bind<i, impl>>
    );

    auto app = injector.create<hello_world>();
}
```

### Main Features
* Header only
* Architecture independent (tested on x86/x86\_64)
* Supports all POSIX and Windows operating systems (tested on Linux 3.8/Windows 7/8)
* Exception safe guaranty
* Thread safe guaranty
* Constructor injection (macro free)
* Compile time creation guaranty
* Compile time policies (arguments permission, binding correctness, circular dependencies, creation ownership, scopes permission)
* Dependencies life time management (scopes: deduce, external, unique, shared, session) + custom scopes
* Scope deduction (shared\_ptr -> shared, unique\_ptr, lvalue -> unique)
* Supports copies, references, pointers, boost and std smart pointers + rvalue references
* Named parameters (named<int, my_int>)
* Runtime visitor (generation UML diagrams)

### Requirements (DI depends on some Boost Libraries)
* Boost.Config
* Boost.SmartPtr
* Boost.MPL
* Boost.Utility
* Boost.TypeTraits
* Boost.Function
* Boost.Bind

### Tested compilers
* Clang 3.2/3.3/3.4
* GCC 4.7.3/4.8.1
* Intel C++ 14.0.0
* MinGW 4.7.3/4.8.1
* Visual Studio 2013 RC / 2013

### Continuous integration
[![Build Status](https://travis-ci.org/krzysztof-jusiak/di.png?branch=master)](https://travis-ci.org/krzysztof-jusiak/di) [![Coverage Status](https://coveralls.io/repos/krzysztof-jusiak/di/badge.png?branch=master)](https://coveralls.io/r/krzysztof-jusiak/di?branch=master)

### More information
[Documentation](http://krzysztof-jusiak.github.com/di/doc/html)
[Todo/Issues] (https://github.com/krzysztof-jusiak/di/issues?state=open)

### License
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

