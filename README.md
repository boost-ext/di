QDeps (C++ dependency injection framework)
================================
    * http://en.wikipedia.org/wiki/Dependency_injection
    * http://www.youtube.com/v/RlfLCWKxHJ0

To get started: git clone --recursive git://github.com/QSrc/QDeps.git

Requirements
------------
    Code:
        * C++ 98 standard-compliant compiler  + typeof extension
        * Boost >= 1.43 (headers only libraries)

    Tests:
        * GNU-compatible Make >= 3.81
        * gtest >= 1.6.0 (UT, MT)
        * [optional] lcov, cppcheck, scan-build, valgrind

Supported compilers (successfully tested)
------------
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
        QDPES_CTOR(C1, shared_ptr<IDummy>, int, Named<int, mpl::string<'port'> >) { }
    };

    struct C2 {
        QDPES_CTOR(C2, const IDummy*, shared_ptr<C1>)
    };

    struct C3 {
        QDPES_CTOR(C3, C2) { }
    };

    // *** base front end *** ------------------------------------------------------------------

    struct BaseModule : Front::Base::Module
        <
            Externals::Bind         < Named<int, mpl::string<'port'> > >,
            Scope<Singleton >::Bind < Dumb >,
            Scope<PerRequest>::Bind < int_<42>, Bind< InCall<C2, C1>, Dumber> >
        >
    { };

    //injector, order in constructor is not important
    Injector<BaseModule> injector(make_shared< Named<int, mpl::string<'port'> >(8080));
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();

    // *** fusion front end *** ----------------------------------------------------------------

    BOOST_AUTO(fusionModule, (Front::Fusion::Module()(
        Bind<IDummy>::To<Dumb>::InScope<Singleton>(),
        Bind<IDummy>::To<Dumber>::InCall<C2, C1>(),
        Inst<int>(make_shared<int>(42)),
        Inst<Named<int, mpl::string<'port'> >(make_shared<int>(8080))
    ));

    Injector<BOOST_TYPEOF(fusionModule)> injector(fusionModule);
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();

    // *** generic front end *** ---------------------------------------------------------------

    struct GenericModule : Front::Generic::Module
        <
            Scope<Singleton>::Bind
            <
                Implementation<IDummy, Dumb>
            >,
            Scope<PerRequest>::Bind
            <
                Implementation<IDummy, Dumber>::Bind< InCall<C2, C1> >,
                Instance<int, int_<42> >,
                Instance<Named<int, mpl::string<'port'> >
            >
        >
    { };

    Injector<GenericModule> injector(make_shared< Named<int, mpl::string<'port'> >(8080));
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();

    // *** path front end *** ------------------------------------------------------------------

    struct PathModule : Front::Path::Module
        <
            PerRequest<int, int_<42> >,
            PerRequest<Named<int, mpl::string<'port'> >,
            Path
            <
                Singleton<IDummy, Dumb>,
                Path
                <
                    PerRequest<IDummy, Dumber>,
                    C1
                >
            >
        >
    { };

    Injector<PathModule> injector(make_shared< Named<int, mpl::string<'port'> >(8080));
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();

    // *** many modules *** --------------------------------------------------------------------

    Injector<BaseModule1, BaseModule2> injector;

    // *** mix modules *** ---------------------------------------------------------------------

    Injector<BaseModule, FusionModule, GenericModule, PathModule> injector;

```

Author
------
Krzysztof Jusiak (krzysztof at jusiak dot net)

TODO
------
    * Method injector
    * Scopes: EagerSingleton, Session
    * Xml front end ?
    * C++11 fork

License
-------
Boost Software License - Version 1.0 - August 17th, 2003

