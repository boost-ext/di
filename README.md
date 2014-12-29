[![Build Status](https://travis-ci.org/krzysztof-jusiak/di.svg?branch=cpp14)](https://travis-ci.org/krzysztof-jusiak/di)

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

Boost.DI is a header only, type safe, compile time, non-intrusive constructor dependency injection
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

**How To Start**

* [Documentation](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html)
* [Issues] (https://github.com/krzysztof-jusiak/di/issues?state=open)

Boost.DI requires C++14 compliant compiler (Clang-3.4+, GCC-5.0+) with STL (memory, new, type\_traits headers).
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

> Let's assume all examples below include `boost/di.hpp` header and define `di` namespace alias.
```cpp
    #include <boost/di.hpp>
    namespace di = boost::di;
```

*

> **Bindings**
```cpp
Create empty injector                   | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector();    | assert(0 == injector.create<int>());
```
```cpp
Bind type to value                      | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | assert(42 == injector.create<int>());
    di::bind<int>.to(42)                |
);                                      |
```
```cpp
Bind type to static value               | Test
----------------------------------------|-----------------------------------------
template<int N> using int_ =            | assert(42 == injector.create<int>());
    std::integral_constant<int, N>;     |
                                        |
auto injector = di::make_injector(      |
    di::bind<int, int_<42>>             |
);                                      |
```
```cpp
Bind interface to implementation        | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | auto object = injector.create<unique_ptr<i>>();
    di::bind<i, impl>                   | assert(dynamic_cast<impl*>(object.get()));
);                                      |
```
```cpp
Bind different interfaces to one        |
implementation                          | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | auto object1 = injector.create<shared_ptr<i1>>();
    di::bind<di::any_of<i1, i2>, impl>  | auto object2 = injector.create<shared_ptr<i2>>();
);                                      | assert(dynamic_cast<impl*>(object1.get()));
                                        | assert(dynamic_cast<impl*>(object2.get()));
                                        | assert(object1 == object2);
```

*

> **Injections**
```cpp
Direct constructor injection            | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    c(int a, double d) : a(a), d(d) { } | assert(42 == object.a);
                                        | assert(87.0 == object.b);
    int a = 0;                          |
    double d = 0.0;                     |
};                                      |
                                        |
auto injector = di::make_injector(      |
    di::bind<int>.to(42)                |
  , di::bind<double>.to(87.0)           |
);                                      |
```
```cpp
Direct constructor injection with many  |
constructors (longest parameters list   |
constructor will be chosen)             | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    c();                                | assert(42 == object.a);
    c(int a) : a(a) { }                 | assert(87.0 == object.b);
    c(int a, double d) : a(a), d(d) { } |
                                        |
    int a = 0;                          |
    double d = 0.0;                     |
};                                      |
                                        |
auto injector = di::make_injector(      |
    di::bind<int>.to(42)                |
  , di::bind<double>.to(87.0)           |
);                                      |
```
```cpp
Direct constructor injection with       |
ambigious constructors (BOOST_DI_INJECT)| Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    c(double d, int a) : a(a), d(d) { } | assert(42 == object.a);
    BOOST_DI_INJECT(c, int a, double d) | assert(87.0 == object.b);
        : a(a), d(d) { }                |
                                        |
    int a = 0;                          |
    double d = 0.0;                     |
};                                      |
                                        |
auto injector = di::make_injector(      |
    di::bind<int>.to(42)                |
  , di::bind<double>.to(87.0)           |
);                                      |
```
```cpp
Direct constructor injection with       |
ambigious constructors                  |
(BOOST_DI_INJECT_TRAITS)                | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    BOOST_DI_INJECT_TRAITS(int, double);| assert(42 == object.a);
    c(double d, int a) : a(a), d(d) { } | assert(87.0 == object.b);
    c(int a, double d) : a(a), d(d) { } |
                                        |
    int a = 0;                          |
    double d = 0.0;                     |
};                                      |
                                        |
auto injector = di::make_injector(      |
    di::bind<int>.to(42)                |
  , di::bind<double>.to(87.0)           |
);                                      |
```
```cpp
Direct constructor injection with       |
ambigious constructors                  |
(di::ctor_traits)                       | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    c(double d, int a) : a(a), d(d) { } | assert(42 == object.a);
    c(int a, double d) : a(a), d(d) { } | assert(87.0 == object.b);
                                        |
    int a = 0;                          |
    double d = 0.0;                     |
};                                      |
                                        |
namespace boost { namespace di {        |
template<>                              |
struct ctor_traits<c> {                 |
    BOOST_DI_INJECT_TRAITS(int, double);|
};                                      |
}} // boost::di                         |
                                        |
auto injector = di::make_injector(      |
    di::bind<int>.to(42)                |
  , di::bind<double>.to(87.0)           |
);                                      |
```

*

> **Scopes**

*

> **Modules**

*

> **Providers**

*

> **Policies**

*

> **Runtime Performance**
```cpp
Create type wihtout bindings            | Asm
----------------------------------------|-----------------------------------------
int main() {                            | xor %eax,%eax
    auto injector = di::make_injector();| retq
    return injector.create<int>();      |
}                                       |
```
```cpp
Create type with bounded instance       | Asm
----------------------------------------|-----------------------------------------
int main() {                            | mov $0x2a,%eax
    auto injector = di::make_injector(  | retq
        di::bind<int>.to(42)            |
    );                                  |
                                        |
    return injector.create<int>();      |
}                                       |
```

*

> **Compile-time Performance**

**License**
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

**Disclaimers**
This is not an official Boost library yet and there is no guarantee it will ever be!

