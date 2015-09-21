<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/krzysztof-jusiak/di/releases/latest" target="_blank">![Github Release](http://img.shields.io/github/release/krzysztof-jusiak/di.svg)</a>
<a href="https://www.biicode.com/krzysztofjusiak/di" target="_blank">![Bii](https://webapi.biicode.com/v1/badges/krzysztofjusiak/krzysztofjusiak/di/master)</a>
<a href="http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/try_it_online.html" target="_blank">![Try Boost.DI Online](https://img.shields.io/badge/try%20boost.di-online-orange.svg)</a>
<a href="https://gitter.im/krzysztof-jusiak/di" target="_blank">![Gitter Chat](https://img.shields.io/badge/gitter-join%20chat%20%E2%86%92-brightgreen.svg)</a>

Boost.DI: C++ Dependency Injection
===============================================
[![Boost Libraries](https://raw.githubusercontent.com/krzysztof-jusiak/di/gh-pages/boost/boost.png)](http://www.boost.org)
<a href="https://travis-ci.org/krzysztof-jusiak/di" target="_blank">![Build Status](https://img.shields.io/travis/krzysztof-jusiak/di/cpp14.svg?label=linux/osx)</a>
<a href="https://ci.appveyor.com/project/krzysztof-jusiak/di" target="_blank">![Build Status](https://img.shields.io/appveyor/ci/krzysztof-jusiak/di/cpp14.svg?label=windows)</a>
<a href="https://coveralls.io/r/krzysztof-jusiak/di?branch=cpp14" target="_blank">![Coveralls](http://img.shields.io/coveralls/krzysztof-jusiak/di/cpp14.svg)</a>
<a href="http://github.com/krzysztof-jusiak/di/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/krzysztof-jusiak/di.svg)</a>

> "Don't call us, we'll call you", Hollywood principle

**Dependency Injection (DI)** involves passing (injecting) one or more dependencies (or services) to a dependent object (or client) which become part of the client’s state.
It is like the Strategy Pattern, except the strategy is set once, at construction. DI enables loosely coupled designs, which are easier to maintain and test.

<p align="center"><a href="http://www.youtube.com/watch?v=IKD2-MAkXyQ"><img src="http://41.media.tumblr.com/c5b2e9096e2b877c856ef549ad211ac0/tumblr_nnzn556v7F1qc0howo5_1280.png" height="200" width="350" ></a></p>

> **"Let's make some coffee!"**
<p align="center"><img src="https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/doc/images/coffee_maker.png" alt="coffee maker"/></p>
```cpp
No Dependency injection                 | Dependency Injection
----------------------------------------|--------------------------------------------
class coffee_maker {                    | class coffee_maker {
public:                                 | public:
    void brew() {                       |   coffee_maker(shared_ptr<iheater> heater
        heater->on();                   |              , unique_ptr<ipump> pump)
        pump->pump();                   |         : heater(heater), pump(move(pump))
        clog << "coffee"! << endl;      |     { }
        heater->off();                  |
    }                                   |     void brew() {
                                        |         heater->on();
private:                                |         pump->pump();
    shared_ptr<iheater> heater =        |         clog << "coffee!" << endl;
        make_shared<electric_heater>(); |         heater->off();
                                        |     }
    unique_ptr<ipump> pump =            |
        make_unique<heat_pump>(heater); | private:
};                                      |     shared_ptr<iheater> heater;
                                        |     unique_ptr<ipump> pump;
                                        | };
                                        |
```

**Why Dependency Injection?**

* DI provides loosely coupled code (separation of business logic and object creation)
* DI provides easier to maintain code (different objects might be easily injected)
* DI provides easier to test code (fakes objects might be injected)

[![The Clean Code Talks - Don't Look For Things!](http://img.youtube.com/vi/RlfLCWKxHJ0/0.jpg)](http://www.youtube.com/watch?v=RlfLCWKxHJ0) | [![DAGGER 2 - A New Type of dependency injection](http://image.slidesharecdn.com/nr73mexetqiybd1qpcem-140529143342-phpapp01/95/dependency-injection-for-android-5-638.jpg?cb=1401392113)](http://www.youtube.com/watch?v=oK_XtfXPkqw) |
--- | --- |

**Why Boost.DI?**

* Boost.DI has none or minimal run-time overhead - [Run-time performance](#run_time_performance)
* Boost.DI compiles fast - [Compile-time performance](#compile_time_performance)
* Boost.DI gives short diagnostic messages - [Diagnostic messages](#diagnostic_messages)
* Boost.DI is not intrusive [link to tutorial]
* Boost.DI reduces boilerplate code [link to tutorial]
* Boost.DI reduces testing effort [link to tutorial]
* Boost.DI gives better control of what and how is created [link to tutorial]
* Boost.DI gives better understanding about objects hierarchy [link to tutorial]

**How To Start?**

* Get C++14 compliant compiler (Clang-3.4+, GCC-5.1+, Visual Studio 2015+) with STL (type\_traits header) / Boost is not required
* [Read Quick User Guide](#quick_user_guide)
* [Read Tutorial](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/tutorial.html)
* [Read Documentation](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html)

> To get started the only file you need is `di.hpp`:

[https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/include/boost/di.hpp](https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/include/boost/di.hpp)

```cpp
    // main.cpp
    #include "di.hpp"
    int main() { }
```

```sh
    $CXX -std=c++14 -I. main.cpp
```

> To get and test Boost.DI library:

```sh
    git clone https://github.com/krzysztof-jusiak/di.git
    cd build && cmake ..
    make all test
```

<a id="quick_user_guide"></a>
**Quick User Guide** | [Examples](https://github.com/krzysztof-jusiak/di/tree/cpp14/example/quick_user_guide)

* [Injector](#injector) | [Bindings](#bindings) | [Injections](#injections) | [Annotations](#annotations) | [Scopes](#scopes) | [Modules](#modules) | [Providers](#providers) | [Policies](#policies)
* [Run-time performance](#run_time_performance) | [Compile-time performance](#compile_time_performance)
* [Diagnostic messages](#diagnostic_messages) | [Configuration](#configuration)

> Let's assume all examples below include `boost/di.hpp` header and define `di` namespace alias.
```cpp
#include <boost/di.hpp>
namespace di = boost::di;
//
struct i1 { virtual ~i1() = default; virtual void dummy1() = 0; };
struct i2 { virtual ~i2() = default; virtual void dummy2() = 0; };
struct impl1 : i1 { void dummy1() override { } };
struct impl2 : i2 { void dummy2() override { } };
struct impl : i1, i2 { void dummy1() override { } void dummy2() override { } };
```

*
<a id="injector"></a>
> **[Injector](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/concepts/injector.html)**
```cpp
Create empty injector                   | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector();    | assert(0 == injector.create<int>());
```

*

<a id="bindings"></a>
> **[Bindings](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/concepts/bindings.html)** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/bindings.cpp) | [More examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/dynamic_bindings.cpp)
```cpp
Bind interface to implementation        | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | auto object = injector.create<unique_ptr<i1>>();
    di::bind<i1>.to<impl1>()            | assert(dynamic_cast<impl1*>(object.get()));
);                                      |
```
```cpp
Bind different interfaces to one        | Test
implementation                          |
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | auto object1 = injector.create<shared_ptr<i1>>();
    di::bind<i1, i2>.to<impl>()         | auto object2 = injector.create<shared_ptr<i2>>();
);                                      | assert(dynamic_cast<impl*>(object1.get()));
                                        | assert(dynamic_cast<impl*>(object2.get()));
```
```cpp
Bind type to value                      | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | assert(42 == injector.create<int>());
    di::bind<int>.to(42)                |
);                                      |
```
```cpp
Cross platform `bind` (Clang/GCC/MSVC)  | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | auto object = injector.create<unique_ptr<i1>>();
    di::bind<i1>().to<impl>()           | assert(dynamic_cast<impl1*>(object.get()));
    // MSVC 2015 doesnt support variable|
    // templates                        |
);                                      |
```
```cpp
Advanced binding                        | Test
----------------------------------------|-----------------------------------------
auto l = 42l;                           | assert(42 == injector.create<int>());
auto b = false;                         | assert(injector.create<shared_ptr<i1>>()
                                        |        ==
auto injector = di::make_injector(      |        injector.create<shared_ptr<i1>>()
   di::bind<int>.to(0)                  | );
 , di::bind<int>.to(42) [di::override]  | assert(l == injector.create<long&>());
 , di::bind<i1>.to(make_shared<impl>()) | assert(&l == &injector.create<long&>());
 , di::bind<long>.to(l)                 | assert(87 == injector.create<short>());
 , di::bind<short>.to([]{return 87;})   | {
 , di::bind<i2>.to(                     | auto object = injector.create<shared_ptr<i2>>();
     [&](const auto& injector)          | assert(nullptr == object);
        -> shared_ptr<i2> {             | }
            if (b) {                    | {
              return injector.template  | b = true;
                create<                 | auto object = injector.create<shared_ptr<i2>>();
                  shared_ptr<impl2>>(); | assert(dynamic_cast<impl2*>(object.get()));
            }                           | }
            return nullptr;             |
     }                                  |
   )                                    |
);                                      |
```

*

<a id="injections"></a>
> **[Injections](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/concepts/injections.html)** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/constructor_injection.cpp) | [More examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/constructor_signature.cpp)
```cpp
Direct constructor injection            | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    c(int a, double d) : a(a), d(d) { } | assert(42 == object.a);
                                        | assert(87.0 == object.d);
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
Aggregate constructor injection         | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    int a = 0;                          | assert(42 == object.a);
    double d = 0.0;                     | assert(87.0 == object.d);
};                                      |
                                        |
auto injector = di::make_injector(      |
    di::bind<int>.to(42)                |
  , di::bind<double>.to(87.0)           |
);                                      |
```
```cpp
Direct constructor injection with many  | Test
constructors (longest parameters list   |
constructor will be chosen)             |
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    c();                                | assert(42 == object.a);
    c(int a) : a(a) { }                 | assert(87.0 == object.d);
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
Direct constructor injection with       | Test
ambiguous constructors (BOOST_DI_INJECT)|
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    c(double d, int a) : a(a), d(d) { } | assert(42 == object.a);
    BOOST_DI_INJECT(c, int a, double d) | assert(87.0 == object.d);
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
Direct constructor injection with       | Test
ambiguous constructors                  |
(BOOST_DI_INJECT_TRAITS)                |
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    BOOST_DI_INJECT_TRAITS(int, double);| assert(42 == object.a);
    c(double d, int a) : a(a), d(d) { } | assert(87.0 == object.d);
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
Direct constructor injection with       | Test
ambiguous constructors                  |
(di::ctor_traits)                       |
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    c(double d, int a) : a(a), d(d) { } | assert(42 == object.a);
    c(int a, double d) : a(a), d(d) { } | assert(87.0 == object.d);
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

<a id="annotations"></a>
> **[Annotations](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/concepts/annotations.html)** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/annotations.cpp)
```cpp
Annotated constructor injection         | Test
----------------------------------------|-----------------------------------------
auto int1 = []{};                       | auto object = injector.create<c>();
auto int2 = []{};                       | assert(42 == object.a);
                                        | assert(87 == object.b);
struct c {                              |
    BOOST_DI_INJECT(c                   |
        , (named = int1) int a          |
        , (named = int2) int b)         |
        : a(a), b(b)                    |
    { }                                 |
                                        |
    int a = 0;                          |
    int b = 0;                          |
};                                      |
                                        |
auto injector = di::make_injector(      |
    di::bind<int>.named(int1).to(42)    |
  , di::bind<int>.named(int2).to(87)    |
);                                      |
```
```cpp
Annotated constructor injection with    | Test
the same names for different parameters |
----------------------------------------|-----------------------------------------
auto n1 = []{};                         | auto object = injector.create<c>();
auto n2 = []{};                         | assert(42 == object.i1);
                                        | assert(42 == object.i2);
struct c {                              | assert(87 == object.i3);
  BOOST_DI_INJECT(c                     | assert(0 == object.i4);
      , (named = n1) int a              | assert("str" == c.s);
      , (named = n1) int b              |
      , (named = n2) int c              |
      , int d                           |
      , (named = n1) string s)          |
  : i1(i1), i2(i2), i3(i3), i4(i4), s(s)|
  { }                                   |
                                        |
  int i1 = 0;                           |
  int i2 = 0;                           |
  int i3 = 0;                           |
  int i4 = 0;                           |
  string s;                             |
};                                      |
                                        |
auto injector = di::make_injector(      |
    di::bind<int>.named(n1).to(42)      |
  , di::bind<int>.named(n2).to(87)      |
  , di::bind<string>.named(n1).to("str")|
);                                      |
```
```cpp
Annotated constructor injection with    | Test
separate constructor definition         |
----------------------------------------|-----------------------------------------
auto int1 = []{};                       | auto object = injector.create<c>();
auto int2 = []{};                       | assert(42 == object.a);
                                        | assert(87 == object.b);
struct c {                              |
    BOOST_DI_INJECT(c                   |
        , (named = int1) int a          |
        , (named = int2) int b);        |
                                        |
    int a = 0;                          |
    int b = 0;                          |
};                                      |
                                        |
c::c(int a, int b) : a(a), b(b) { }     |
                                        |
auto injector = di::make_injector(      |
    di::bind<int>.named(int1).to(42)    |
  , di::bind<int>.named(int2).to(87)    |
);                                      |
```
```cpp
Annotated constructor injection with    | Test
di::ctor_traits                         |
----------------------------------------|-----------------------------------------
auto int1 = []{};                       | auto object = injector.create<c>();
auto int2 = []{};                       | assert(42 == object.a);
                                        | assert(87 == object.b);
struct c {                              |
    c(int a, int b) : a(a), b(b) { }    |
                                        |
    int a = 0;                          |
    int b = 0;                          |
};                                      |
                                        |
namespace boost { namespace di {        |
template<>                              |
struct ctor_traits<c> {                 |
    BOOST_DI_INJECT_TRAITS(             |
        (named = int1) int              |
      , (named = int2) int);            |
};                                      |
}} // boost::di                         |
                                        |
auto injector = di::make_injector(      |
    di::bind<int>.named(int1).to(42)    |
  , di::bind<int>.named(int2).to(87)    |
);                                      |
```

*

<a id="scopes"></a>
> **[Scopes](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/concepts/scopes.html)** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/deduce_scope.cpp) | [More examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/custom_scope.cpp)
```cpp
Deduce scope (default)                  | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object1 = injector.create<unique_ptr<c>>();
    shared_ptr<i1> sp; /*singleton*/    | auto object2 = injector.create<unique_ptr<c>>();
    unique_ptr<i2> up; /*unique*/       | assert(object1->sp == object2->sp);
    int& i; /*singleton*/               | assert(object1->up != object2->up);
    double d; /*unique*/                | assert(42 == object1->i);
};                                      | assert(&i == &object1->i;
                                        | assert(42 == object2->i);
auto i = 42;                            | assert(&i == &object2->i);
                                        | assert(87.0 == object1->d);
auto injector = di::make_injector(      | assert(87.0 == object2->d);
    di::bind<i1>.to<impl1>()            |
  , di::bind<i2>.to<impl2>()            |
  , di::bind<int>.to(i)                 |
  , di::bind<double>.to(87.0)           |
);                                      |
```
| Type | Deduced scope |
|------|-------|
| T | unique |
| T& | singleton |
| const T& | unique (temporary)/singleton |
| T* | unique (ownership transfer) |
| const T* | unique (ownership transfer) |
| T&& | unique |
| unique\_ptr<T> | unique |
| shared\_ptr<T> | singleton |
| weak\_ptr<T> | singleton |
```cpp
Unique scope                            | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | assert(injector.create<shared_ptr<i1>>()
 di::bind<i1>.in(di::unique).to<impl1>()|        !=
);                                      |        injector.create<shared_ptr<i1>>()
                                        | );
```
```cpp
Singleton scope (shared between threads)| Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | assert(injector.create<shared_ptr<i1>>()
    di::bind<i1>.                       |        ==
        in(di::singleton).to<impl1>()   |        injector.create<shared_ptr<i1>>()
);                                      | );
```
```cpp
Custom scope                            | Test
----------------------------------------|-----------------------------------------
struct custom_scope {                   | assert(injector.create<shared_ptr<i1>>()
  template<class TExpected, class>      |        !=
  struct scope {                        |        injector.create<shared_ptr<i1>>()
    template<class T, class TProvider>  | );
    auto create(const TProvider& pr) {  |
      return                            |
        shared_ptr<TExpected>{pr.get()};|
    }                                   |
  };                                    |
};                                      |
                                        |
auto injector = di::make_injector(      |
    di::bind<i1>.                       |
        in(custom_scope{}).to<impl1>()  |
);                                      |
```
| Type/Scope | unique | singleton | external |
|------------|--------|--------|-----------|---------|----------|
| T | ✔ | - | ✔ |
| T& | - | ✔  | ✔ |
| const T& | ✔ (temporary) | ✔ | ✔ |
| T* (transfer ownership) | ✔ | - | - | - | ✔ |
| const T* | ✔ | - | ✔ |
| T&& | ✔ | - | - |
| unique\_ptr<T> | ✔ |  - | ✔ |
| shared\_ptr<T> | ✔ | ✔ | ✔ |
| weak\_ptr<T> | - | ✔ | ✔ |

*

<a id="modules"></a>
> **[Modules](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/concepts/modules.html)** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/modules.cpp)
```cpp
Module                                  | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<unique_ptr<c>>();
    c(unique_ptr<i1> i1                 | assert(dynamic_cast<impl1*>(object->i1.get()));
    , unique_ptr<i2> i2                 | assert(dynamic_cast<impl2*>(object->i2.get()));
    , int i) : i1(move(i1))             | assert(42 == object->i);
             , i2(move(i2)), i(i)       |
    { }                                 | auto up1 = injector.create<unique_ptr<i1>>();
                                        | assert(dynamic_cast<impl1*>(up1.get()));
    unique_ptr<i1> i1;                  |
    unique_ptr<i2> i2;                  | auto up2 = injector.create<unique_ptr<i2>>();
    int i = 0;                          | assert(dynamic_cast<impl2*>(up2.get()));
};                                      |
                                        |
struct module1 {                        |
    auto configure() const noexcept {   |
        return di::make_injector(       |
            di::bind<i1>.t<impl1>()     |
          , di::bind<int>.to(42)        |
        );                              |
    }                                   |
};                                      |
                                        |
struct module2 {                        |
    auto configure() const noexcept {   |
        return di::make_injector(       |
            di::bind<i2>.to<impl2>()    |
        );                              |
    };                                  |
};                                      |
                                        |
auto injector = di::make_injector(      |
    module1{}, module2{}                |
);                                      |
```
```cpp
Exposed type module                     | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    c(shared_ptr<i1> i1                 | assert(dynamic_cast<impl1*>(object.i1.get()));
    , shared_ptr<i2> i2                 | assert(dynamic_cast<impl2*>(object.i2.get()));
    , int i) : i1(i1), i2(i2), i(i)     | assert(42 == object.i);
    { }                                 |
                                        | // injector.create<unique_ptr<i1>>() // compile error
    shared_ptr<i1> i1;                  | // injector.create<unique_ptr<i2>>() // compile error
    shared_ptr<i2> i2;                  |
    int i = 0;                          |
};                                      |
                                        |
struct module {                         |
    di::injector<c> configure()         |
    const noexcept;                     |
                                        |
    int i = 0;                          |
};                                      |
                                        |
di::injector<c> // expose c             |
module::configure() const noexcept {    |
    return di::make_injector(           |
        di::bind<i1>.to<impl1>()        |
      , di::bind<i2>.to<impl2>()        |
      , di::bind<int>.to(i)             |
    );                                  |
}                                       |
                                        |
auto injector = di::make_injector(      |
    module{42}                          |
);                                      |
```
```cpp
Exposed many types module               | Test
----------------------------------------|-----------------------------------------
struct module {                         | auto up1 = injector.create<unique_ptr<i1>>();
    di::injector<i1, i2> configure()    | assert(dynamic_cast<impl1*>(up1.get()));
    const noexcept;                     |
                                        | auto up2 = injector.create<unique_ptr<i2>>();
    int i = 0;                          | assert(dynamic_cast<impl2*>(up2.get()));
};                                      |
                                        |
di::injector<i1, i2> // expose i1, i2   |
module::configure() const noexcept {    |
    return di::make_injector(           |
        di::bind<i1>.to<impl1>()        |
      , di::bind<i2>.to<impl2>()        |
    );                                  |
}                                       |
                                        |
auto injector = di::make_injector(      |
    module{}                            |
);                                      |
```
```cpp
Exposed type module with annotation     | Test
----------------------------------------|-----------------------------------------
auto my = []{};                         | auto object = injector.create<unique_ptr<c>>();
                                        | assert(dynamic_cast<impl1*>(object->up.get()));
struct c {                              |
    BOOST_DI_INJECT(c                   |
      , (named = my) unique_ptr<i1> up) |
      : up(up)                          |
    { }                                 |
                                        |
    unique_ptr<i1> up;                  |
};                                      |
                                        |
di::injector<i1> module =               |
    di::make_injector(                  |
        di::bind<i1>.to<impl1>()        |
    );                                  |
                                        |
auto injector = di::make_injector(      |
    di::bind<i1>.named(my).to(module)   |
);                                      |
```

*

<a id="providers"></a>
> **[Providers](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/concepts/providers.html)** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/custom_provider.cpp)
* [heap](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/concepts/providers/heap.html)
* [stack\_over\_heap (default)](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/concepts/providers/stack_over_heap_default.html)
```cpp
Heap no throw provider                  | Test
----------------------------------------|-----------------------------------------
class heap_no_throw {                   | // per injector policy
public:                                 | auto injector = di::make_injector<my_provider>();
  template<                             | assert(0 == injector.create<int>());
    class // interface                  |
  , class T // implementation           | // global policy
  , class TInit // direct()/uniform{}   | #define BOOST_DI_CFG my_provider
  , class TMemory // heap/stack         | auto injector = di::make_injector();
  , class... TArgs>                     | assert(0 == injector.create<int>());
  auto get(const TInit&                 |
         , const TMemory&               |
         , TArgs&&... args)             |
  const noexcept {                      |
      return new (nothrow)              |
        T{forward<TArgs>(args)...};     |
  }                                     |
};                                      |
                                        |
class my_provider : public di::config { |
public:                                 |
    auto provider() const noexcept {    |
        return heap_no_throw{};         |
    }                                   |
};                                      |
```

*

<a id="policies"></a>
> **[Policies](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/concepts/policies.html)** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/types_dumper.cpp) | [More examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/custom_policy.cpp)
```cpp
Define policies configuration           | Test
(dump types)                            |
----------------------------------------|-----------------------------------------
class print_types_policy                | // per injector policy
    : public di::config {               | auto injector = di::make_injector<print_types_policy>();
public:                                 | injector.create<int>(); // output: int
  auto policies() const noexcept {      |
    return di::make_policies(           | // global policy
      [](auto type){                    | #define BOOST_DI_CFG my_policy
         using T = decltype(type);      | auto injector = di::make_injector();
         using arg = typename T::type;  | injector.create<int>(); // output: int
         cout << typeid(arg).name()     |
              << endl;                  |
      }                                 |
    );                                  |
  }                                     |
};                                      |
```
```cpp
Define policies configuration           | Test
(dump types extended)                   |
----------------------------------------|-----------------------------------------
class print_types_info_policy           | // per injector policy
    : public di::config {               | auto injector = di::make_injector<print_types_info_policy>(
public:                                 |     di::bind<i1>.to<impl1>()
  auto policies() const noexcept {      | );
    return di::make_policies(           |
      [](auto type                      | injector.create<unique_ptr<i1>>();
       , auto dep                       |
       , auto... ctor) {                | // output:
         using T = decltype(type);      |     0 // ctor_size of impl1
         using arg = typename T::type;  |     unique_ptr<i1> // ctor arg
         using arg_name =               |     di::no_name // ctor arg name
            typename T::name;           |     di::deduce // scope
         using D = decltype(dep);       |     i1 // expected
         using scope =                  |     impl1 // given
            typename D::scope;          |     no_name // dependency
         using expected =               |
            typename D::expected;       |
         using given =                  | // global policy
            typename D::given;          | #define BOOST_DI_CFG my_policy
         using name =                   | auto injector = di::make_injector(
            typename D::name;           |     di::bind<i1>.to<impl1>()
         auto ctor_s = sizeof...(ctor); | );
                                        |
         cout << ctor_s                 | injector.create<unique_ptr<i1>>();
              << endl                   |
              << typeid(arg).name()     | // output:
              << endl                   |     0 // ctor_size of impl1
              << typeid(arg_name).name()|     unique_ptr<i1> // cotr arg
              << endl                   |     di::no_name // ctor arg name
              << typeid(scope).name()   |     di::deduce // scope
              << endl                   |     i1 // expected
              << typeid(expected).name()|     impl1 // given
              << endl                   |     no_name // dependency
              << typeid(given).name()   |
              << endl                   |
              << typeid(name).name()    |
              << endl;                  |
         ;                              |
      }                                 |
    );                                  |
  }                                     |
};                                      |
|
```
```cpp
`constructible` policy                  | Test
----------------------------------------|-----------------------------------------
#include <boost/di/                     | // global policy
    policies/constructible.hpp>         | #define BOOST_DI_CFG all_must_be_bound_unless_int
                                        | assert(0 == di::make_injector().create<int>());
class all_must_be_bound_unless_int      |
    : public di::config {               | // di::make_injector().create<double>(); // compile error
public:                                 | assert(42.0 == make_injector(
  auto policies() const noexcept {      |                    di::bind<double>.to(42.0)
    using namespace di::policies;       |                ).create<double>()
    using namespace                     | );
        di::policies::operators;        |
                                        |
    return di::make_policies(           |
      constructible(                    |
        is_same<_, int>{} ||            |
        is_bound<_>{})                  |
    );                                  |
  }                                     |
};                                      |
    |
```

*

<a id="run_time_performance"></a>
> **[Run-time performance (-O2)](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/performance.html)**
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
struct module {                         | push   %rbx
	di::injector<i1> configure() const {| mov    0x2007e9(%rip),%al
		return di::make_injector(       | test   %al,%al
			di::bind<i1>.to<impl1>()    | jne    0x4009be <main+46>
		);                              | mov    $0x601180,%edi
	}                                   | callq  0x4007e0 <__cxa_guard_acquire@plt>
};                                      | test   %eax,%eax
                                        | je     0x4009be <main+46>
int main() {                            | movq   $0x400a00,0x2007b4(%rip
	auto injector = di::make_injector(  | mov    $0x601180,%edi
        module{}                        | callq  0x400820 <__cxa_guard_release@plt>
    );                                  | mov    0x2007a3(%rip),%rax
                                        | mov    $0x601168,%edi
	auto ptr = injector.create<         | callq  *%rax
        unique_ptr<i1>                  | test   %rax,%rax
    >();                                | setne  %cl
                                        | movzbl %cl,%ebx
	return ptr != nullptr;              | je     0x4009e0 <main+80>
}                                       | mov    (%rax),%rcx
                                        | mov    %rax,%rdi
                                        | callq  *0x8(%rcx)
                                        | mov    %ebx,%eax
                                        | pop    %rbx
                                        | retq
                                        | mov    %rax,%rdi
                                        | callq  0x4009f0 <__clang_call_terminate>
```

*

<a id="compile_time_performance"></a>
> **[Compile-time performance](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/performance.html)** | [Example](https://github.com/krzysztof-jusiak/di/blob/cpp14/test/pt/di.cpp)
* Environment
    * x86\_64 Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz GenuineIntel GNU/Linux
    * clang++3.4 -O2
```cpp
Boost.DI header                         | Time [s]
----------------------------------------|-----------------------------------------
#include <boost/di.hpp>                 | 0.110
int main() { }                          |
```
```cpp
Legend:
    ctor    = raw constructor: c(int i, double d);
    inject  = inject constructor: BOOST_DI_INJECT(c, int i, double d);
    all     = all types exposed from module: auto configure();
    exposed = one type exposed from module: di::injector<c> configure();
```
[![small complexity](https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/doc/images/small_complexity.png)](https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/doc/images/small_complexity.png)
```cpp
* 4248897537 instances created
* 132 different types
* 10 modules
```
---
[![medium complexity](https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/doc/images/medium_complexity.png)](https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/doc/images/medium_complexity.png)
```cpp
* 1862039751439806464 instances created
* 200 different types
* 10 modules
```
---
[![big complexity](https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/doc/images/big_complexity.png)](https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/doc/images/big_complexity.png)
```cpp
* 5874638529236910091 instances created
* 310 different types
* 100 different interfaces
* 10 modules
```

*

<a id="diagnostic_messages"></a>
> **[Diagnostic messages](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/diagnostic_messages.html)**
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

<a id="configuration"></a>
> **[Configuration](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/overview.html#di.overview.configuration)**
```cpp
Macro                                   | Description
----------------------------------------|-----------------------------------------
BOOST_DI_CFG_CTOR_LIMIT_SIZE            | Limits number of allowed consturctor
                                        | parameters [0-10, default=10]
----------------------------------------|-----------------------------------------
BOOST_DI_CFG                            | Global configuration allows to customize
                                        | provider and policies
```

**Contributing**
* Extensions (e.g. see [extensions](example/extensions))
* Core (for example. bug fixes / improvements of compilation/run-time times)

---

[![Stories in Progress](https://badge.waffle.io/krzysztof-jusiak/di.svg??label=waffle%3Ain%20progress&title=In%20Progress)](http://waffle.io/krzysztof-jusiak/di)
[![Throughput Graph](https://graphs.waffle.io/krzysztof-jusiak/di/throughput.svg)](https://waffle.io/krzysztof-jusiak/di/metrics)

---

**Disclaimer** Boost.DI is not an official Boost library.

