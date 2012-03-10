QDeps (C++ dependency injection framework)
================================

QDeps is a C++ dependency injection framework (http://en.wikipedia.org/wiki/Dependency_injection).

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

Supported compilers (succesfully tested)
------------
    * gcc-3.4.6 - gcc-4.6.2
    * clang3

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

    struct Dummy : IDummy {
        virtual void dummy() { }
    };

    struct Smart : IDummy {
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
            Externals::Bind         < Named<int, mpl::string<'port'> >           >,
            Scope<Singleton >::Bind < Dummy                                      >,
            Scope<PerRequest>::Bind < int_<42>, Bind< InCall<C2, C1>, Smart>     >
        >
    { };

    //injector, order in constructor is not important
    Injector<BaseModule> injector(make_shared< Named<int, mpl::string<'port'> >(new int(8080)));
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();

    // *** fusion front end *** ----------------------------------------------------------------

    BOOST_AUTO(fusionModule, (Front::Fusion::Module()(
        bind<IDummy>::to<Dummy>::inScope<Singleton>(),
        bind<IDummy>::to<Smart>::InCall<C2, C1>(),
        inst<int>(make_shared<int>(42)),
        inst<Named<int, mpl::string<'port'> >(make_shared<int>(8080))
    ));

    Injector<BOOST_TYPEOF(fusionModule)> injector(fusionModule);
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();

    // *** generic front end *** ---------------------------------------------------------------

    struct GenericModule : Front::Generic::Module
        <
            Scope<Singleton>::Bind
            <
                Impl<IDummy, Dummy>
            >,
            Scope<PerRequest>::Bind
            <
                Impl<IDummy, Smart>::Bind< InCall<C2, C1> >,
                Inst<int, int_<42> >,
                Inst<Named<int, mpl::string<'port'> >
            >
        >
    { };

    Injector<GenericModule> injector(make_shared< Named<int, mpl::string<'port'> >(new int(8080)));
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();

    // *** path front end *** ------------------------------------------------------------------

    struct PathModule : Front::Path::Module
        <
            PerRequest<int, int_<42> >,
            PerRequest<Named<int, mpl::string<'port'> >,
            Path
            <
                Singleton<IDummy, Dummy>,
                Path
                <
                    PerRequest<IDummy, Smart>,
                    C1
                >
            >
        >
    { };

    Injector<PathModule> injector(make_shared< Named<int, mpl::string<'port'> >(new int(8080)));
    shared_ptr<C3> l_sp = injector.create< shared_ptr<C3> >();
    C3 l_value = injector.create<C3>();

    // *** many modules *** --------------------------------------------------------------------

    Injector<BaseModule1, BaseModule2> injector;
    C3 l_value = injector.create<C3>();

    // *** mix modules *** ---------------------------------------------------------------------

    Injector<BaseModule, GenericModule, PathModule> injector;

```

Author
------
Krzysztof Jusiak (krzysztof at jusiak dot net)

TODO
------
    * Method injector
    * C++11 fork
    * Xml front end ?

License
-------
Boost Software License - Version 1.0 - August 17th, 2003

