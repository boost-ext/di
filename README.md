C++ Dependency Injection Framework
================================
* [Dependency Injection](http://en.wikipedia.org/wiki/Dependency_injection)
* [The Clean Code Talks - Don't Look For Things!](http://www.youtube.com/watch?v=RlfLCWKxHJ0&feature=BFa&list=PLED6CA927B41FF5BD)
* [Goolge Guice - dependency injection framework for Java ](http://code.google.com/p/google-guice/)

To get started
-----
`git clone git://github.com/qsrc/di.git`

Documentation
-----
* [wiki](http://qsrc.github.com/di)

Usage
-----

``` C++
#include <di/di.hpp>

struct num_of_limits { };
struct allocator : iallocator { DI_CTOR(allocator, int, shared_ptr<load>) { } };
struct data : idata { DI_CTOR(named<capacity_limit, down>, named<capacity_limit, up>) { } };
struct storage { DI_CTOR_TRAITS(named<int, up>, float); storage(int, float) { } };
template<> struct ctor_traits<load> { static void ctor(shared_ptr<storage>); };
struct app { DI_CTOR(storage, const shared_ptr<limit_checker>&) { } };
...
```

``` C++
struct simple_module : base::module <                           // simple module
    per_requests <                                              // new instance each time
        allocator                                               // of iallocator -> allocator
    >,
    singletons <                                                // the same instance
        data, storage                                           // of idata->data, storage
    >
> { };

app obj = injector<simple_module>().create<app>();              // create app and all dependencies
```

``` C++
BOOST_AUTO(fusion_module, fusion::module()(                     // simple fusion module
    singletons <
        storage
    >(),
    per_requests <
        bind<Limit>::in_name<on>::in_call_stack<storage, load>, // bind (in name) only when storage and
        priority_queue                                          // load were created in given order
    >(),
    bind<int>::in_call_stack<selector>::to(87)                  // bind external value
));

injector<BOOST_TYPEOF(fusion_module)> inj(fusion_module);       // install fusion module

shared_ptr<app> obj = injector.create< shared_ptr<app> >();     // create app as shared_ptr
```

``` C++
typedef base::module <                                          // complex base module
    per_requests <                                              // always new instance
        bind<imap, map>,                                        // bind imap to map implementation
        data,                                                   // bind data to interface
        allocator,                                              // from which data is inhereting
        bind<limit_checker>::in_call_stack<capacity>            // bind implementation limit_checker
    >,                                                          // only when capacity class is created
    singletons <
        bind<capacity_limit>::in_name<down>,                    // bind using named parameter
        bind<capacity_limit>::in_name<up>
    >,
    singleton<calculator>,                                      // one line notation - singleton
    per_request<isetup, setup>,                                 // one line notation - per_request
    scope<custom_scope>::bind <                                 // custom scope
        stack
    >,
    externals <                                                 // outside objects
        iconfig,
        annotate< bind<int>::in_call_stack<C> >::with<up_int>   // bind to annotation - simplify setting
    >
> base_module;

injector<base_module> injector(                                 // create injector from 2 modules
    base_module(                                                // initialize base_module externals
        base_module::set<iconfig>(config),                      // initialize iconfig by Config
        base_module::set<up_int>(42)
    )
);

shared_ptr<app> obj = injector.create< shared_ptr<app> >();     // create app as shared_ptr
```

``` C++
injector<> empty_injector;                                      // default empty injector

BOOST_AUTO(injector, empty_injector.install(                    // install 2 modules
    base_module(
        base_module::set<iconfig>(config),
        base_module::set<up_int>(42)
    ),
    fusion_module
);

app obj = injector.create<app>();                               // and create App as lvalue
```

``` C++
class text_visitor
{
    template<typename T> void operator()() const
    {
        std::cout << typeid(typename T::type).name()            // ex: boost::shared_ptr<I>
          << ", " << typeid(typename T::expected).name()        // ex: I
          << ", " << typeid(typename T::given).name()           // ex: Impl
          << ", " << typeid(typename T::context).name();        // ex: vector<C1, C2>
    }
};

injector.visit<C3>(text_visitor());                             // apply text_visitor for C3
```

``` C++
struct action {
    DI_CTOR(action, shared_ptr<data>, shared_ptr<msg_sender>) { ... }
    template<typename Event> void operator()(const event&) { }
};

struct guard {
    DI_CTOR(guard, shared_ptr<common>) { ... }
    template<typename Event> bool operator()(const event&) { return false; }
};

class example : public qfsm::fsm
{
    class s1 { };
    class s2 { };

public:
    typedef qfsm::transition_table
    <
        transition < s1 , e1 , s2 , action , guard >
    >
    transition_table;
};

qfsm::front::fsm<example> fsm =
    injector<>().create<front::fsm<example> >();                // create fsm with actions and guards
```
Features
-----
    * injection via constructor
    * configuration via front ends (base, fusion)
    * binding via interface/implementation
    * binding via is_base_of (smart binding)
    * binding via call_stack (path binding)
    * binding via external objects
    * binding via named parameters
    * providers
    * scopes (per_request, singleton, session, custom scope)
    * modules
    * visitors
    * policies (ex. circular dependencies detection)
    * compile time approach (supports C++98, C++11)

Requirements
------------
    code:
        * c++ 98 standard-compliant compiler supporting BOOST_TYPEOF
        * boost >= 1.43 (needed files from boost_1_49 are in Externals/boost)

    tests:
        * gnu-compatible Make >= 3.81
        * [optional] lcov, cppcheck, scan-build, valgrind

    documentation:
        * vim >= 7.3
        * vimwiki >= 1.2 (vimwiki 1.2 is in Externals/vimwiki)

    supported compilers (successfully tested):
        * gcc   >= 4.3
        * clang >= 3.0

Tests & Examples & Doc & Diagnostics
------------
    make                # test cov cppcheck scan-build valgrind doc examples
    make test           # compile and run only UT/MT
    make examples       # build examples

Install
------------
    [sudo] make install   # copy di to /usr/include

C++ Dependency Injection Frameworks
------------
* https://github.com/qsrc/di
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

