QDeps - C++ dependency injection framework (headers only library)
================================
* Dependency Injection (http://en.wikipedia.org/wiki/Dependency_injection)
* The Clean Code Talks - Don't Look For Things!
    (http://www.youtube.com/watch?v=RlfLCWKxHJ0&feature=BFa&list=PLED6CA927B41FF5BD)

        git clone --recursive git://github.com/QSrc/QDeps.git

Usage
-----

``` C++
#include <QDeps/QDeps.hpp>

struct NumOfLimits { };
struct Allocator { QDEPS_CTOR(Allocator, int, double) { } };
struct Measurements { QDEPS_CTOR(Named<CapacityLimit, Down>, Named<CapacityLimit, Up>) { } };
struct Storage { QDEPS_CTOR_TRAITS(Named<int, Up>., shared_ptr<Selector>); Storage(int, shared_ptr<Selector>) { } };
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
        Bind<CapacityLimit>::InName<Up>,
        Calculator
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
        l_fusionModule                                          // fusion module is already created
    );

    shared_ptr<App> sp = injector.create< shared_ptr<App> >();  // create App as shared_ptr
}

{
    Injector<> l_injector;                                      // default empty injector

    App lvalue = l_injector.install(                            // install 2 modules
        BaseModule(                                             // and create App as lvalue
            Set<IConfig>(Config),
            Set<UpInt>(42)
        ),
        l_fusionModule
    ).create<App>();
}

```
Requirements
------------
    Code:
        * C++ 98 standard-compliant compiler  + typeof extension
        * Boost >= 1.43 (headers only libraries)

    Tests:
        * GNU-compatible Make >= 3.81
        * gtest >= 1.6.0 (UT, MT)
        * [optional] lcov, cppcheck, scan-build, valgrind

    Supported compilers (successfully tested):
        * gcc   >= 3.4.6
        * clang >= 3.0
        * icc   >= 11.1
        * msvc  ?

Tests & Examples & Doc & Analysis
------------
    make                # test cov cppcheck scan-build valgrind doc examples
    make test           # compile and run only UT/MT
    make doc            # generate documentation
    make examples       # build examples

Install
------------
    sudo make install   # copy QDeps to /usr/include

TODO
------
    * visitor -> dump dot
    * C++11 fork

Author
------
Krzysztof Jusiak (krzysztof at jusiak dot net)

License
-------
Boost Software License - Version 1.0 - August 17th, 2003

