QDeps - C++ dependency injection framework
================================
* Dependency Injection (http://en.wikipedia.org/wiki/Dependency_injection)
* The Clean Code Talks - Don't Look For Things! (http://www.youtube.com/watch?v=RlfLCWKxHJ0&feature=BFa&list=PLED6CA927B41FF5BD)

        git clone --recursive git://github.com/QSrc/QDeps.git

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

Usage
-----

``` C++
#include <QDeps/Front/Ctor.hpp>
#include <QDeps/Utility/Injector.hpp>
#include <QDeps/Utility/Named.hpp>

struct IDummy { virtual ~I() { } virtual void dummy() = 0; };
struct Dumb : IDummy { virtual void dummy() { } };
struct Dumber : IDummy { virtual void dummy() { } };
struct C1 { QDPES_CTOR(C1, shared_ptr<IDummy> /*Dumber per request*/, int /*42*/, Named<int, string<'port'> > /*8080 external*/) { } };
struct C2 { QDPES_CTOR(C2, const IDummy*, shared_ptr<C1> /*Dumb singleton with C3*/) };
struct C3 { QDPES_CTOR(C3, C2, const shared_ptr<IDummy>& /*Dumb singleton with C2*/) { } };

// *** base front end ***
{
    #include <QDeps/Front/Base/Module.hpp>

    struct BaseModule : Module <
        Singletons <
            Dumb
        >,
        PerRequests <
            int_<42>,
            Bind<Dumber>::InCall<C2, C1>
        >,
        Externals <
            Annotate<Bind<int>::InName<Port> >::With<string<'port'> >
        >
    > { };

    typedef Injector<BaseModule> Injector;
    Injector injector(
        Injector::Set<string<'port'> >(8080)
    );
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();
}

// *** fusion front end ***
{
    #include <QDeps/Front/Fusion/Module.hpp>

    BOOST_AUTO(fusionModule, (Module()(
        Singletons <
            Dumb
        >(),
        PerRequests <
            int_<42>,
            Bind<Dumber>::InCall<C2, C1>
        >(),
        Bind<int>::InName<string<'port'> >::To(8080)
    )));

    Injector<BOOST_TYPEOF(fusionModule)> injector(fusionModule);
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();
}

```

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

