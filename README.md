QDeps - C++ dependency injection framework (headers only library)
================================
* Dependency Injection (http://en.wikipedia.org/wiki/Dependency_injection)
* The Clean Code Talks - Don't Look For Things! (http://www.youtube.com/watch?v=RlfLCWKxHJ0&feature=BFa&list=PLED6CA927B41FF5BD)

        git clone --recursive git://github.com/QSrc/QDeps.git

Usage
-----

``` C++
#include <QDeps/QDeps.hpp>

struct NumOfLimits { };
struct Allocator { QDEPS_CTOR(Allocator, int, double) { } };
struct Measurements { QDEPS_CTOR(Named<CapacityLimit, Down> p_down, Named<CapacityLimit, Up> p_up) { } };
struct Storage { QDEPS_CTOR_TRAITS(Named<int, Up>); Storage(int p_i) { } };
...

struct BaseModule : Base::Module <
    Singletons <
        Bind<CapacityLimit>::InName<Down>,
        Bind<CapacityLimit>::InName<Up>,
        Bind<NumOfCapacityLimit>::InName<Limit>,
        ResourceCalculator
    >,
    PerRequests <
        Bind<IMap, Map>,
        Measurements,
        Allocator,
        CapacityLimit,
        NumOfLimits,
        Bind<LimitChecker>::InCall<Capacity>
    >,
    Externals <
        IConfig,
        Annotate<Bind<int>::InName<Up> >::With<UpInt>
    >
> { };

BOOST_AUTO(l_fusionModule, Fusion::Module()(
    Singletons <
        Storage
    >(),
    PerRequests <
        Bind<ServicesLimit>::InName<Limits>::InCall<Selector, LoadTracker>,
        PriorityQueue,
        PriorityMultiqueue,
        Selector,
        LoadTracker
    >(),
    Bind<int>::InCall<Selector>::To(87)
));

{
    Injector<BaseModule, BOOST_TYPEOF(l_fusionModule)> injector(
        BaseModule(
            Set<IConfig>(Config),
            Set<UpInt>(42)
        ),
        l_fusionModule
    );

    shared_ptr<App> sp = injector.create< shared_ptr<App> >();
}

{
    Injector<> l_injector;

    l_injector.install(
        BaseModule(
            Set<IConfig>(Config),
            Set<UpInt>(42)
        ),
        l_fusionModule
    );

    App lvalue = injector.create<App>();
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

