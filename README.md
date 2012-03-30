QDeps - C++ dependency injection framework (headers only library)
================================
* [Dependency Injection](http://en.wikipedia.org/wiki/Dependency_injection)
* [The Clean Code Talks - Don't Look For Things!](http://www.youtube.com/watch?v=RlfLCWKxHJ0&feature=BFa&list=PLED6CA927B41FF5BD)
* [Goolge Guice - dependency injection framework for Java ](http://code.google.com/p/google-guice/)

To get started
-----
        git clone --recursive git://github.com/QSrc/QDeps.git

Usage
-----

``` C++
#include <QDeps/QDeps.hpp>

struct NumOfLimits { };
struct Allocator { QDEPS_CTOR(Allocator, int, shared_ptr<Load>) { } };
struct Measurements { QDEPS_CTOR(Named<CapacityLimit, Down>, Named<CapacityLimit, Up>) { } };
struct Storage { QDEPS_CTOR_TRAITS(Named<int, Up>, float); Storage(int, float) { } };
struct App { QDEPS_CTOR(Storage, const shared_ptr<LimitChecker>&) { } };
...

struct BaseModule : Base::Module <                              // base module : type
    PerRequests <                                               // always new instance
        Bind<IMap, Map>,                                        // bind IMap to Map implementation
        Measurements,                                           // bind Measurements to interface
        Allocator,                                              // from which Measurements is inhereting
        Bind<LimitChecker>::InCall<Capacity>                    // bind implementation LimitChecker
    >,                                                          // only when Capacity class is created
    Singletons <
        Bind<CapacityLimit>::InName<Down>,                      // bind using Named parameter
        Bind<CapacityLimit>::InName<Up>
    >,
    Singleton<Calculator>,                                      // one line notation - Singleton
    PerRequest<Setup>,                                          // one line notation - PerRequest
    Scope<CustomScope>::Bind <                                  // custom scope
        Stack
    >,
    Externals <                                                 // outside objects
        IConfig,
        Annotate<Bind<int>::InName<Up> >::With<UpInt>           // bind to annotation - simplify setting
    >
> { };

BOOST_AUTO(fusionModule, Fusion::Module()(                      // fusion module : object
    Singletons <
        Storage
    >(),
    PerRequests <
        Bind<Limit>::InName<On>::InCall<Storage, Load>,         // bind (in name) only when Storage and
        PriorityQueue                                           // Load were created in given order
    >(),
    Bind<int>::InCall<Selector>::To(87)                         // bind external value
));

{
    Injector<BaseModule, BOOST_TYPEOF(fusionModule)> injector(  // create injector from 2 modules
        BaseModule(                                             // initialize BaseModule externals
            Set<IConfig>(Config),                               // initialize IConfig by Config
            Set<UpInt>(42)
        ),
        fusionModule                                            // fusion module is already created
    );

    shared_ptr<App> app = injector.create< shared_ptr<App> >(); // create App as shared_ptr
}

{
    Injector<> emptyInjector;                                   // default empty injector

    BOOST_AUTO(injector, emptyInjector.install(                 // install 2 modules
        BaseModule(
            Set<IConfig>(Config),
            Set<UpInt>(42)
        ),
        fusionModule
    );

    App app = injector.create<App>();                           // and create App as lvalue
}

```
Requirements
------------
    Code:
        * c++ 98 standard-compliant compiler + typeof extension
        * boost >= 1.43 (headers only libraries)

    Tests:
        * gnu-compatible Make >= 3.81
        * gtest >= 1.6.0 (UT, MT)
        * [optional] lcov, cppcheck, scan-build, valgrind

    Documentation:
        * vim >= 7.3

    Supported compilers (successfully tested):
        * gcc   >= 3.4.6
        * clang >= 3.0
        * icc   >= 11.1
        * msvc  ?

Tests & Examples & Doc & Analysis
------------
    make                # test cov cppcheck scan-build valgrind doc examples
    make test           # compile and run only UT/MT
    make examples       # build examples

Install
------------
    [sudo] make install   # copy QDeps to /usr/include

Author
------
Krzysztof Jusiak (krzysztof at jusiak dot net)

License
-------
Boost Software License - Version 1.0 - August 17th, 2003

