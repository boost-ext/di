QDeps - C++ Dependency Injection Framework
================================
* [Dependency Injection](http://en.wikipedia.org/wiki/Dependency_injection)
* [The Clean Code Talks - Don't Look For Things!](http://www.youtube.com/watch?v=RlfLCWKxHJ0&feature=BFa&list=PLED6CA927B41FF5BD)
* [Goolge Guice - dependency injection framework for Java ](http://code.google.com/p/google-guice/)

To get started
-----
`git clone git://github.com/QSrc/QDeps.git`

Documentation
-----
* [Wiki](http://qsrc.github.com/QDeps)

Usage
-----

``` C++
#include <QDeps/QDeps.hpp>

struct NumOfLimits { };
struct Allocator { QDEPS_CTOR(Allocator, int, shared_ptr<Load>) { } };
struct Data { QDEPS_CTOR(Named<CapacityLimit, Down>, Named<CapacityLimit, Up>) { } };
struct Storage { QDEPS_CTOR_TRAITS(Named<int, Up>, float); Storage(int, float) { } };
struct App { QDEPS_CTOR(Storage, const shared_ptr<LimitChecker>&) { } };
...
```

``` C++
typedef Base::Module <                                          // base module : type
    PerRequests <                                               // always new instance
        Bind<IMap, Map>,                                        // bind IMap to Map implementation
        Data,                                                   // bind Data to interface
        Allocator,                                              // from which Data is inhereting
        Bind<LimitChecker>::InCall<Capacity>                    // bind implementation LimitChecker
    >,                                                          // only when Capacity class is created
    Singletons <
        Bind<CapacityLimit>::InName<Down>,                      // bind using Named parameter
        Bind<CapacityLimit>::InName<Up>
    >,
    Singleton<Calculator>,                                      // one line notation - Singleton
    PerRequest<ISetup, Setup>,                                  // one line notation - PerRequest
    Scope<CustomScope>::Bind <                                  // custom scope
        Stack
    >,
    Externals <                                                 // outside objects
        IConfig,
        Annotate<Bind<int>::InName<Up> >::With<UpInt>           // bind to annotation - simplify setting
    >
> BaseModule;

Injector<BaseModule> injector(                                  // create injector from 2 modules
    BaseModule(                                                 // initialize BaseModule externals
        BaseModule::Set<IConfig>(config),                       // initialize IConfig by Config
        BaseModule::Set<UpInt>(42)
    )
);

shared_ptr<App> app = injector.create< shared_ptr<App> >();     // create App as shared_ptr
```

``` C++
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

Injector<BOOST_TYPEOF(fusionModule)> injector(fusionModule);    // install fusion module

shared_ptr<App> app = injector.create< shared_ptr<App> >();     // create App as shared_ptr
```

``` C++
Injector<> emptyInjector;                                       // default empty injector

BOOST_AUTO(injector, emptyInjector.install(                     // install 2 modules
    BaseModule(
        BaseModule::Set<IConfig>(Config),
        BaseModule::Set<UpInt>(42)
    ),
    fusionModule
);

App app = injector.create<App>();                               // and create App as lvalue
```

``` C++
class TextVisitor
{
    template<typename T> void operator()() const
    {
        std::cout << typeid(typename T::Type).name()            // ex: boost::shared_ptr<I>
          << ", " << typeid(typename T::Expected).name()        // ex: I
          << ", " << typeid(typename T::Given).name()           // ex: Impl
          << ", " << typeid(typename T::Context).name();        // ex: vector<C1, C2>
    }
};

injector.visit<C3>(TextVisitor());                              // apply TextVisitor for C3
```

``` C++
struct Action {
    QDEPS_CTOR(Action, shared_ptr<Data>, shared_ptr<MsgSender>) { ... }
    template<typename Event> void operator()(const Event&) { }
};

struct Guard {
    QDEPS_CTOR(Guard, shared_ptr<Common>) { ... }
    template<typename Event> bool operator()(const Event&) { return false; }
};

class Example : public QFsm::Fsm
{
    class S1 { };
    class S2 { };

public:
    typedef QFsm::TransitionTable
    <
        Transition < S1 , e1 , S2 , Action , Guard >
    >
    TransitionTable;
};

Front::Fsm<Example> fsm =
    Injector<>().create<Front::Fsm<Example> >();                // create fsm with actions and guards
```
Features
-----
    * Injection via constructor
    * Configuration via front ends (base, fusion)
    * Binding via interface/implementation
    * Binding via is_base_of (smart binding)
    * Binding via call_stack (path binding)
    * Binding via external objects
    * Binding via named parameters
    * Providers
    * Scopes (PerRequest, Singleton, Session, Custom Scopes)
    * Modules
    * Visitors
    * Policies (ex. circular dependencies detection)
    * Compile time approach (supports C++98, C++11)

Requirements
------------
    Code:
        * c++ 98 standard-compliant compiler supporting BOOST_TYPEOF
        * boost >= 1.43 (needed files from boost_1_49 are in Externals/boost)

    Tests:
        * gnu-compatible Make >= 3.81
        * [optional] lcov, cppcheck, scan-build, valgrind

    Documentation:
        * vim >= 7.3
        * vimwiki >= 1.2 (vimwiki 1.2 is in Externals/vimwiki)

    Supported compilers (successfully tested):
        * gcc   >= 4.3
        * clang >= 3.0

Tests & Examples & Doc & Diagnostics
------------
    make                # test cov cppcheck scan-build valgrind doc examples
    make test           # compile and run only UT/MT
    make examples       # build examples

Install
------------
    [sudo] make install   # copy QDeps to /usr/include

C++ Dependency Injection Frameworks
------------
* https://github.com/QSrc/QDeps
* https://github.com/phs/sauce
* https://bitbucket.org/cheez/dicpp
* http://code.google.com/p/spring-cpp
* http://code.google.com/p/autumnframework
* http://code.google.com/p/ffead-cpp
* http://sourceforge.net/projects/qtioccontainer
* http://sourceforge.net/projects/cpp-resolver

Author
------
Krzysztof Jusiak (krzysztof at jusiak dot net)

License
-------
Boost Software License - Version 1.0 - August 17th, 2003

