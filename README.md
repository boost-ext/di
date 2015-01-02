[![Build Status](https://travis-ci.org/krzysztof-jusiak/di.svg?branch=cpp14)](https://travis-ci.org/krzysztof-jusiak/di)

### C++ Dependency Injection
> "Don't call us, we'll call you", Hollywood principle

Dependency injection is a programming practice providing required instances to an object.

* Provides loosely coupled code (separation of business logic and object creation)
* Provides easier to maintain code (different objects might be easily injected)
* Provides easier to test code (fakes objects might be injected)

```cpp
No Dependency injection                 | Dependency Injection
----------------------------------------|--------------------------------------------
class example {                         | class example {
public:                                 | public:
    example()                           |     example(shared_ptr<ilogic> logic
        : logic_(new logic())           |           , shared_ptr<ilogger> logger)
        , logger_(factory::create())    |         : logic_(logic), logger_(logger)
    { }                                 |     { }
                                        |
    int run() const;                    |     int run() const;
                                        |
private:                                | private:
    shared_ptr<ilogic> logic_;          |     shared_ptr<ilogic> logic_;
    shared_ptr<ilogger> logger_;        |     shared_ptr<ilogger> logger_;
};                                      | };
                                        |
```
Boost.DI is a header only, type safe, compile time, non-intrusive constructor dependency injection
library improving manual dependency injection by simplifying object instantiation with automatic
dependencies injection.

* Reduces boilerplate code (no factories, no objects creation)
* Reduces cost of maintenance effort (constructor signature change won't affect di configuration)
* Reduces testing effort (automatic mocks Injector)
* Gives better control of what and how is created (policies, providers)
* Gives better understanding about objects hierarchy (Types creation graph)

```cpp
Manual Dependency Injection             | Boost.DI
----------------------------------------|--------------------------------------------
int main() {                            | int main() {
    /*boilerplate code*/                |     auto injector = di::make_injector(
    auto logic = make_shared<logic>();  |         di::bind<ilogic, logic>
    auto logger = make_shared<logger>();|       , di::bind<ilogger, logger>
                                        |     );
    return example{logic, logger}.run();|     return injector.create<example>().run();
}                                       |}
```

**Why Dependency Injection?**

[![The Clean Code Talks - Don't Look For Things!](http://img.youtube.com/vi/RlfLCWKxHJ0/0.jpg)](http://www.youtube.com/watch?v=RlfLCWKxHJ0)

**How To Start?**

* Get C++14 compliant compiler (Clang-3.4+, GCC-5.0+) with STL (memory, type\_traits headers) / Boost is not required
* Read Quick User Guide (below)
* [Read tutorial](http://krzysztof-jusiak.github.io/di/boost/libs/di/doc/html/di/tutorial.html)
* [Read documentation](http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html)
* [Check/Report Issues] (https://github.com/krzysztof-jusiak/di/issues?state=open)

> To get started the only file you need is `di.hpp`:

[https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/include/boost/di.hpp](https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/include/boost/di.hpp)

```cpp
    // main.cpp
    #include "di.hpp"
    int main() { }
```

```sh
    $CXX -std=c++1y -I. main.cpp
```

> To get and test Boost.DI library:

```sh
    git clone --recursive https://github.com/krzysztof-jusiak/di.git
    cd di/build
    ./bootstrap.sh --with-toolset=clang
    ./b2 -j4 ../test ../example
```

**Quick User Guide** | [Hello World](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/hello_world.cpp)

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

> **Bindings** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/binding.cpp) | [More examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/dynamic_binding.cpp)

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
auto injector = di::make_injector(      | auto object = injector.create<unique_ptr<i1>>();
    di::bind<i1, impl1>                 | assert(dynamic_cast<impl1*>(object.get()));
);                                      |
```
```cpp
Bind different interfaces to one        | Test
implementation                          |
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | auto object1 = injector.create<shared_ptr<i1>>();
    di::bind<di::any_of<i1, i2>, impl>  | auto object2 = injector.create<shared_ptr<i2>>();
);                                      | assert(dynamic_cast<impl*>(object1.get()));
                                        | assert(dynamic_cast<impl*>(object2.get()));
                                        | assert(object1 == object2);
```
```cpp
Bind to external value                  | Test
----------------------------------------|-----------------------------------------
auto i = 42;                            | auto object = injector.create<const int&>();
                                        | assert(i == object);
auto injector = di::make_injector(      | assert(&i == &object);
    di::bind<int>.to(std::cref(i));     |
);                                      |
```

*

> **Injections** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/constructor_injection.cpp) | [More examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/constructor_signature.cpp) | [Even more examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/multiple_interfaces.cpp)
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
Aggregate constructor injection         | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    int a = 0;                          | assert(42 == object.a);
    double d = 0.0;                     | assert(87.0 == object.b);
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
Direct constructor injection with       | Test
ambigious constructors (BOOST_DI_INJECT)|
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
Direct constructor injection with       | Test
ambigious constructors                  |
(BOOST_DI_INJECT_TRAITS)                |
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
Direct constructor injection with       | Test
ambigious constructors                  |
(di::ctor_traits)                       |
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

> **Annotations** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/named.cpp)
```cpp
Annotated constructor injection         | Test
----------------------------------------|-----------------------------------------
auto int1 = []{};                       | auto object = injector.create<c>();
auto int2 = []{};                       | assert(42 == c.a);
                                        | assert(87 == c.b);
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
the same names for different variables  |
----------------------------------------|-----------------------------------------
auto n1 = []{};                         | auto object = injector.create<c>();
auto n2 = []{};                         | assert(42 == c.a);
                                        | assert(42 == c.b);
struct c {                              | assert(87 == c.c);
    BOOST_DI_INJECT(c                   | assert(0 == c.d);
        , (named = n1) int a            | assert("str" == c.s);
        , (named = n1) int b            |
        , (named = n2) int c            |
        , int d                         |
        , (named = n1) string s)        |
        : a(a), b(b), c(c), d(d), s(s)  |
    { }                                 |
                                        |
    int a = 0;                          |
    int b = 0;                          |
    int c = 0;                          |
    int d = 0;                          |
    string s;                           |
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
seperate constructor definition         |
----------------------------------------|-----------------------------------------
auto int1 = []{};                       | auto object = injector.create<c>();
auto int2 = []{};                       | assert(42 == c.a);
                                        | assert(87 == c.b);
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
ctor_traits
----------------------------------------|-----------------------------------------
auto int1 = []{};                       | auto object = injector.create<c>();
auto int2 = []{};                       | assert(42 == c.a);
                                        | assert(87 == c.b);
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

> **Scopes** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/deduce_scope.cpp) | [More examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/custom_scope.cpp) | [Even more examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/externals.cpp)
```cpp
Deduce scope (default)                  | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object1 = injector.create<c>();
    shared_ptr<i1> sp; /*singleton*/    | auto object2 = injector.create<c>();
    unique_ptr<i2> up; /*unique*/       | assert(object1.sp == object2.sp);
    int& i; /*external*/                | assert(object1.up != object2.up);
    double d; /*unique*/                | assert(42 == object1.i);
};                                      | assert(&i == object1.i;
                                        | assert(42 == object2.i);
auto i = 42;                            | assert(&i == object2.i);
                                        | assert(87.0 == object1.d);
auto injector = di::make_injector(      | assert(87.0 == object2.d);
    di::bind<i1, impl1>                 |
  , di::bind<i2, impl2>                 |
  , di::bind<int>.to(std::ref(i))       |
  , di::bind<double>.to(87.0)           |
);                                      |
```
```cpp
Unique scope                            | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | assert(injector.create<shared_ptr<i1>>()
    di::bind<i1, impl1>.in(di::unique)  |        !=
);                                      |        injector.create<shared_ptr<i1>>()
                                        | );
```
```cpp
Shared scope (shared per one thread)    | Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | assert(injector.create<shared_ptr<i1>>()
    di::bind<i1, impl1>.in(di::shared)  |        ==
);                                      |        injector.create<shared_ptr<i1>>()
                                        | );
```
```cpp
Singleton scope (shared between threads)| Test
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | assert(injector.create<shared_ptr<i1>>()
   di::bind<i1, impl1>.in(di::singleton)|        ==
);                                      |        injector.create<shared_ptr<i1>>()
                                        | );
```
```cpp
Session scope                           | Test
----------------------------------------|-----------------------------------------
auto my_session = []{};                 | assert(nullptr == injector.create<shared_ptr<i1>>());
                                        |
auto injector = di::make_injector(      | injector.call(di::session_entry(my_session);
    di::bind<i1, impl1>.in(             |
        di::session(my_mession)         | assert(injector.create<shared_ptr<i1>>()
    )                                   |        ==
);                                      |        injector.create<shared_ptr<i1>>()
                                        | );
                                        |
                                        | injector.call(di::session_exit(my_session);
                                        |
                                        | assert(nullptr == injector.create<shared_ptr<i1>>());
```
```cpp
External scope                          | Test
----------------------------------------|-----------------------------------------
auto l = 42l;                           | assert(42 == injector.create<int>()); // external has priority
auto b = false;                         | assert(injector.create<shared_ptr<i1>>()
                                        |        ==
auto injector = di::make_injector(      |        injector.create<shared_ptr<i1>>()
   di::bind<int, int_<41>>              |
 , di::bind<int>.to(42)                 | );
 , di::bind<i1>.to(make_shared<impl>());| assert(l == injector.create<long&>());
 , di::bind<long>.to(std::ref(l);       | assert(&l == &injector.create<long&>());
 , di::bind<short>.to([]{return 87;})   | assert(87 == injector.create<short>());
 , di::bind<i2>.to(                     | {
     [&](const auto& injector) {        | auto object = injector.create<shared_ptr<i2>>();
        if (b) {                        | assert(nullptr == object);
            return injector.template    | }
                create<impl2>();        | {
        }                               | b = true;
        return nullptr;                 | auto object = injector.create<shared_ptr<i2>>();
     }                                  | assert(dynamic_cast<impl2*>(object.get()));
);                                      | }
```

**Scope deduction**

| Type | Scope |
|------|-------|
| T | unique |
| T& | error - has to be bound as external |
| const T& | unique (temporary) |
| T* | unique (ownership transfer) |
| const T* | unique (ownership transfer) |
| T&& | unique |
| unique\_ptr<T> | unique |
| shared\_ptr<T> | singleton |
| weak\_ptr<T> | singleton |

**Scope to type conversion**

| Type/Scope | unique | shared | singleton | session | external |
|------------|--------|--------|-----------|---------|----------|
| T | ✔ | - | - | - | ✔ |
| T& | - | - | - | - | ✔ |
| const T& | - | - | - | - | ✔ |
| T* | ✔ | - | - | - | ✔ |
| const T* | ✔ | - | - | - | ✔ |
| T&& | ✔ | - | - | - | - |
| unique\_ptr<T> | ✔ | - | - | - | ✔ |
| shared\_ptr<T> | ✔ | ✔ | ✔ | ✔ | ✔ |
| weak\_ptr<T> | - | ✔ | ✔ | ✔ | - |
||
| Temporary const T& | ✔ | - | - | - | - |

*

> **Modules** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/modules.cpp)
```cpp
Module                                  | Test
----------------------------------------|-----------------------------------------
struct c {                              | auto object = injector.create<c>();
    c(unique_ptr<i1> i1                 | assert(dynamic_cast<impl1*>(object.i1.get()));
    , unique_ptr<i2> i2                 | assert(dynamic_cast<impl2*>(object.i2.get()));
    , int i) : i1(i1), i2(i2), i(i)     | asert(42 == object.i);
    { }                                 |
                                        | assert(dynamic_cast<impl1*>(injector.crate<unique_ptr<i1>>());
    unique_ptr<i1> i1;                  | assert(dynamic_cast<impl2*>(injector.crate<unique_ptr<i2>>());
    unique_ptr<i2> i2;                  |
    int i = 0;                          |
};                                      |
                                        |
struct module1 {                        |
    auto configure() const noexcept {   |
        return di::make_injector(       |
            di::bind<i1, impl1>         |
          , di::bind<int>.to(42)        |
        );                              |
    }                                   |
};                                      |
                                        |
struct module2 {                        |
    auto configure() const noexcept {   |
        return di::make_injector(       |
            di::bind<i2, impl2>         |
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
struct module {                         | auto object = injector.create<c>();
    di::injector<c> configure()         | assert(dynamic_cast<impl1*>(object.i1.get()));
    const noexcept;                     | assert(dynamic_cast<impl2*>(object.i2.get()));
                                        | asert(42 == object.i);
    int i = 0;                          |
};                                      | injector.crate<unique_ptr<i1>>() // compile error
                                        | injector.crate<unique_ptr<i2>>() // compile error
di::injector<c>                         |
module::configure() const noexcept {    |
    return di::make_injector(           |
        di::bind<i1, impl1>             |
      , di::bind<i2, impl2>             |
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
struct module {                         | assert(dynamic_cast<impl1*>(injector.crate<unique_ptr<i1>>());
    di::injector<i1, i2> configure()    | assert(dynamic_cast<impl2*>(injector.crate<unique_ptr<i2>>());
    const noexcept;                     |
                                        |
    int i = 0;                          |
};                                      |
                                        |
di::injector<i1, i2>                    |
module::configure() const noexcept {    |
    return di::make_injector(           |
        di::bind<i1, impl1>             |
      , di::bind<i2, impl2>             |
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
auto my = []{};                         | auto object = injector.create<c>();
                                        | assert(dynamic_cast<impl1*>(c.up.get()));
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
        di::bind<i1, impl1>             |
    );                                  |
                                        |
auto injector = di::make_injector(      |
    di::bind<i1>.named(my).to(module)   |
);                                      |
```

*

> **Policies** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/types_dumper.cpp) | [More examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/custom_policy.cpp)
```cpp
Define policies configuration           | Test
----------------------------------------|-----------------------------------------
class print_types_policy                | // per injector policy
    : public di::config {               | auto injector = di::make_injector<print_types_policy>();
public:                                 | injector.create<int>(); // output: int
  auto policies() const noexcept {      |
    return di::make_policies(           | // global policy
      [](auto type){                    | #define BOOST_DI_CFG my_policy
        using T =                       | auto injector = di::make_injector();
          typename decltype(type)::type;| injector.create<int>(); // output: int
                                        |
        std::cout                       |
            << typeid(T).name()         |
            << std::endl;               |
      }                                 |
    );                                  |
  }                                     |
};                                      |
```
```cpp
Allow ctor types policy                 | Test
----------------------------------------|-----------------------------------------
#include <boost/di/                     | // global policy
    policies/allow_ctor_types.hpp>      | #define BOOST_DI_CFG all_must_be_bound_unless_int
                                        | assert(0 == auto injector.create<int>());
class all_must_be_bound_unless_int      |
    : public di::config {               | injector.create<double>(); // compile error
public:                                 | assert(87.0, make_injector(di::bind<double>.to(42.0)).create<double>());
  auto policies() const noexcept {      |
    using namespace di::policies;       |
    using namespace                     |
        di::policies::operators;        |
                                        |
    return di::make_policies(           |
      allow_ctor_types(                 |
        std::is_same<_, int>{} ||       |
        is_bound<_>{})                  |
    );                                  |
  }                                     |
};                                      |
```

*

> **Providers (heap, stack_over_heap - default)** | [Examples](https://github.com/krzysztof-jusiak/di/blob/cpp14/example/custom_provider.cpp)
```cpp
Stack/Heap no throw provider            | Test
----------------------------------------|-----------------------------------------
class stack_heap_no_throw {             | // per injector policy
public:                                 | auto injector = di::make_injector<my_provider>();
  template<                             | assert(0 == injector.create<int>());
    class T                             |
  , class TInit // direct()/aggregate{} | // global policy
  , class TMemory // heap/stack         | #define BOOST_DI_CFG my_provider
  , class... TArgs>                     | auto injector = di::make_injector();
  auto* get(const TInit&                | assert(0 == injector.create<int>());
          , const TMemory&              |
          , TArgs&&... args)            |
  const noexcept {                      |
      return new (std::nothrow)         |
        T{std::forward<TArgs>(args)...};|
  }                                     |
};                                      |
                                        |
class my_provider : public di::config { |
public:                                 |
    auto provider() const noexcept {    |
        return stack_heap_no_throw{};   |
    }                                   |
};                                      |
```

*

> **Run-time performance (-O2)**
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
        di::bind<i1, impl1>             | callq  0x4007b0 <_Znwm@plt>
    );                                  | movq   $0x400a30,(%rax)
                                        | mov    $0x8,%esi
    auto ptr = injector.create<         | mov    %rax,%rdi
        std::unique_ptr<i1>             | callq  0x400960 <_ZdlPvm>
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
			di::bind<i1, impl1>         | movq   $0x400a10,(%rax)
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
        std::unique_ptr<i1>             |
    >();                                |
                                        |
	return ptr != nullptr;              |
}                                       |
```
```cpp
Create bound interface via exposed      | Asm x86-64
module                                  | cost = virtual call due to type erasure
----------------------------------------|-----------------------------------------
struct module {                         | push   %rbp
	di::injector<i1> configure() const {| push   %rbx
		return di::make_injector(       | sub    $0x38,%rsp
			di::bind<i1, impl1>         | lea    0x10(%rsp),%rdi
		);                              | lea    0x8(%rsp),%rsi
	}                                   | callq  0x400bf0 <_ZN5boost2di4core8injectorINS0_3aux9type_listIJNS1_10dependencyINS0_6scopes7exposedINS6_6deduceEEE2i1SA_NS0_7no_nameELb0EEEEEENS0_6configEEC2IJ6moduleEEEDpRKT_>
};                                      | mov    0x18(%rsp),%rdi
                                        | mov    (%rdi),%rax
int main() {                            | lea    0x30(%rsp),%rsi
	auto injector = di::make_injector(  | callq  *0x10(%rax)
        module{}                        | test   %rax,%rax
    );                                  | setne  %bpl
                                        | je     0x400b57 <main+55>
	auto ptr = injector.create<         | mov    (%rax),%rcx
        std::unique_ptr<i1>             | mov    %rax,%rdi
    >();                                | callq  *0x8(%rcx)
                                        | mov    0x20(%rsp),%rbx
	return ptr != nullptr;              | test   %rbx,%rbx
}                                       | je     0x400bcd <main+173>
                                        | lea    0x8(%rbx),%rax
                                        | mov    $0x0,%ecx
                                        | test   %rcx,%rcx
                                        | je     0x400b82 <main+98>
                                        | mov    $0xffffffff,%ecx
                                        | lock   xadd %ecx,(%rax)
                                        | mov    %ecx,0x30(%rsp)
                                        | mov    0x30(%rsp),%ecx
                                        | jmp    0x400b89 <main+105>
                                        | mov    (%rax),%ecx
                                        | lea    -0x1(%rcx),%edx
                                        | mov    %edx,(%rax)
                                        | cmp    $0x1,%ecx
                                        | jne    0x400bcd <main+173>
                                        | mov    (%rbx),%rax
                                        | mov    %rbx,%rdi
                                        | callq  *0x10(%rax)
                                        | lea    0xc(%rbx),%rax
                                        | mov    $0x0,%ecx
                                        | test   %rcx,%rcx
                                        | je     0x400bb8 <main+152>
                                        | mov    $0xffffffff,%ecx
                                        | lock   xadd %ecx,(%rax)
                                        | mov    %ecx,0x30(%rsp)
                                        | mov    0x30(%rsp),%ecx
                                        | jmp    0x400bbf <main+159>
                                        | mov    (%rax),%ecx
                                        | lea    -0x1(%rcx),%edx
                                        | mov    %edx,(%rax)
                                        | cmp    $0x1,%ecx
                                        | jne    0x400bcd <main+173>
                                        | mov    (%rbx),%rax
                                        | mov    %rbx,%rdi
                                        | callq  *0x18(%rax)
                                        | movzbl %bpl,%eax
                                        | add    $0x38,%rsp
                                        | pop    %rbx
                                        | pop    %rbp
                                        | retq
```

*

> **Compile-time performance (time clang++ -O2)**
```cpp
Boost.DI header                         | Time [s] / Size [kb]
----------------------------------------|-----------------------------------------
#include <boost/di.hpp>                 | 0.25s / 6.2 kb
int main() { }                          |
```
```cpp
Number of bindings                      | Time [s] / Size [kb]
----------------------------------------|-----------------------------------------
```
```cpp
Scope deduction                         | Time [s] / Size [kb]
----------------------------------------|-----------------------------------------
```
```cpp
Automatic injection                     | Time [s] / Size [kb]
----------------------------------------|-----------------------------------------
```
```cpp
BOOST_DI_INJECT                         | Time [s] / Size [kb]
----------------------------------------|-----------------------------------------
```

*

> **Error messages (-ftemplate-backtrace-limit=1 -ferror-limit=1)**
```cpp
Create interface without bound          | Error message
implementation                          |
----------------------------------------|-----------------------------------------
auto injector = di::make_injector();    | error: allocating an object of abstract class type 'i1' return new (std::nothrow) T{std::forward<TArgs>(args)...};
injector.create<unique_ptr<i1>>();      |
```
```cpp
Ambiguous binding                       | Error message
----------------------------------------|-----------------------------------------
auto injector = di::make_injector(      | error: base class 'boost::di::aux::pair<boost::di::aux::pair<int, boost::di::no_name>, std::integral_constant<bool, true> >' specified more than once as a direct base class
    di::bind<int>.to(42)                |
  , di::bind<int>.to(87)                |
);                                      |
                                        |
injector.create<int>();                 |
```
```cpp
Create not bound object with all bound  | Error message
policy                                  |
----------------------------------------|-----------------------------------------
class all_bound : public di::config {   | error: static_assert failed "Type T is not allowed" static_assert(decltype(T::apply(data)){}, "Type T is not allowed");
public:                                 |
  auto policies() const noexcept {      |
    return di::make_policies(           |
      allow_ctor_types(is_bound<_>{})   |
    );                                  |
  }                                     |
};                                      |
                                        |
auto injector =                         |
    di::make_injector<all_bound>();     |
injector.create<int>();                 |
```
```cpp
Wrong annotation                        | Error message
(NAMED instead of named)                |
----------------------------------------|-----------------------------------------
auto name = []{};                       | error: use of undeclared identifier 'named'
                                        |
struct c {                              |
    BOOST_DI_INJECT(c                   |
        , (NAMED = name) int) { }       |
};                                      |
                                        |
di::make_injector().create<c>();        |
```

**Similar libraries**
* C++ | [dicpp](https://bitbucket.org/cheez/dicpp) | [Fruit](https://github.com/google/fruit)
* Java | [Dagger 2](https://github.com/google/dagger) | [Guice](https://github.com/google/guice)
* C# | [Ninject](http://www.ninject.org)

**License**
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

<table border="1" style="width:100%">
  <tr>
    <td>Jill</td>
    <td>Smith</td> 
    <td>50</td>
  </tr>
  <tr>
    <td>Eve</td>
    <td>Jackson</td> 
    <td>94</td>
  </tr>
</table>

