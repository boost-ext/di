**C++ Dependency Injection Framework**

### Motivation Example

```cpp
#include <boost/di.hpp>

namespace di = boost::di;

struct i { virtual ~i() { } };
struct impl : i { };
struct c
{
    BOOST_DI_CTOR(c,
        std::shared_ptr<i> p1       // i->impl, p1 == hello_world.p1
      , double p2                   // p2 = 0.0
    );
};

struct hello_world
{
    BOOST_DI_CTOR(hello_world
        , std::shared_ptr<i> p1     // i->impl
        , std::unique_ptr<i> p3     // i->impl, p3 != p1 != c.p1
        , c p4                      // by value
        , int p5                    // p5 = 0
    ) { }
};

int main() {
    using injector = di::injector<
        impl // -> di::deduce<di::bind<i, impl>> -> di::singleton<di::bind<i, impl>>
    >;

    auto app = injector().create<hello_world>();
}

```

### Features
 + supports dependency injection by constructors
 + supports scopes (deduce, per\_request, singleton, session) + custom scopes
 + supports scope deduction from parameter by default (ex: shared\_ptr -> singleton, unique\_ptr -> per\_request)
 + supports C++03 and C++11 standard
 + supports c++11 smart pointers, move semantics and combinations between boost and std smart pointers
 + supports named parameters (named<int, my\_int>>
 + supports creation of dynamic dependent factories
 + supports binding external values (passed by the client)
 + supports generation of constructor traits via script (to avoid using BOOST\_DI\_CTOR macro)
 + supports compile time policies (circular dependencies, creation ownership, not safe arguments)
 + supports module structure (injectors might be used separately or together)
 + supports run time visitor through the objection creation (ex: generation UML diagrams)
 + supports mocks testing facilities (to create mock hierarchy)
 + minimal overhead / compile time approach

### Tested
 + Visual Studio 2013 RC
 + Clang 3.3
 + GCC 4.8.1
 + MinGW 4.8.1

