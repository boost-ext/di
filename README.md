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

Tests & Analysis
------------
    cd Test && make         # test cov cppcheck scan-build valgrind
    cd Test && make test    # compile and run only UT/MT

Usage
-----

``` C++

#include <QDeps/Front/Inject.hpp>
#include <QDeps/Utility/Injector.hpp>
#include <QDeps/Utility/Named.hpp>

struct IDummy {
    virtual ~I() { }
    virtual void dummy() = 0;
};

struct Dumb : IDummy {
    virtual void dummy() { }
};

struct Dumber : IDummy {
    virtual void dummy() { }
};

struct C1 {
    QDPES_CTOR(C1, shared_ptr<IDummy> /*Dumber per request*/,
                   int /*42*/,
                   Named<int, string<'port'> > /*8080 external*/
    ) { }
};

struct C2 {
    QDPES_CTOR(C2, const IDummy*, shared_ptr<C1> /*Dumb singleton with C3*/)
};

struct C3 {
    QDPES_CTOR(C3, C2, const shared_ptr<IDummy>& /*Dumb singleton with C2*/) { }
};

// *** base front end ***
{
    #include <QDeps/Front/Base/Module.hpp>

    struct BaseModule : Front::Base::Module
        <
            Externals::Bind         < Named<int, string<'port'> > >,
            Scope<Singleton >::Bind < Dumb >,
            Scope<PerRequest>::Bind < int_<42>, Bind<Dumber>::InCall<C2, C1> >
        >
    { };

    //injector, order in constructor is not important
    Injector<BaseModule> injector(make_shared< Named<int, string<'port'> >(8080));
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();
}

// *** fusion front end ***
{
    #include <QDeps/Front/Fusion/Module.hpp>

    BOOST_AUTO(fusionModule, (Front::Fusion::Module()(
        Bind<IDummy>::To<Dumb>::InScope<Singleton>(),
        Bind<IDummy>::To<Dumber>::InCall<C2, C1>(),
        Inst<int>(make_shared<int>(42)),
        Inst<Named<int, string<'port'> >(make_shared<int>(8080))
    ));

    Injector<BOOST_TYPEOF(fusionModule)> injector(fusionModule);
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();
}

// *** many modules ***
{
    Injector<BaseModule1, BaseModule2> injector;
}

// *** mix modules ***
{
    Injector<BaseModule, FusionModule> injector;
}

```

TODO
------
    * make install
    * Scopes: EagerSingleton, Session, TestScope
    * Method injector ?
    * Xml front end ?
    * C++11 fork

Author
------
Krzysztof Jusiak (krzysztof at jusiak dot net)

License
-------
Boost Software License - Version 1.0 - August 17th, 2003

