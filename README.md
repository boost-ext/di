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

Motiviation example
------------

Usage
-----

``` C++
    class C1
    {
    public:
        QDPES_CTOR(Ctor, shared_ptr<I1> i1, int i, Attr<std::string, string<'test'> > s)
            : i1(i1), i(i), s(s)
        { }

    private:
        shared_ptr<I1> i1;
        int i;
        std::string s;
    };

    //base front end
    struct BaseModule : Front::Base::Module
        <
            Externals::Bind         < C1, Named<int, dupa>                                        >,
            Scope<Singleton >::Bind < Impl1, Bind<Interface, Impl2>, Bind< InCall<C1, C2>, Impl2> >,
            Scope<PerRequest>::Bind < int_<32>, mpl::string<'text'>                               >
        >
    { };

    //fusion front end
    BOOST_AUTO(fusionModule, (Front::Fusion::Module()(
        bind<I>::to<Impl>(),
        inst<int>(make_shared<int>(32)),
        bind<I1>::to<Im1>::inScope<Singleton>::inCall<C1, C2>()
    ));

    //generic front end
    struct GenericModule : Front::Generic::Module
        <
            Scope<Singleton>::Bind                                      //one instantion
            <
                Impl<I3, Impl3>,
                Inst<Attr<int, string<'Port'> >, int_<5> >,             //set to 5
            >,
            Scope<PerRequest>::Bind
            <
                Impl<I1, Impl1>,                                        //per request
                Impl<I1, ImplI11>::Bind<C2>                             //custom bind I1 to C2
                Inst<int, int_<42> >::Bind<C4, CallStack<C1, C2> >      //bind int=42 to C4 and C1->C2
            >
        >
    { };

    //path front end
    struct PathModule : Front::Path::Module
        <
            Singleton<I3, Impl3>,
            PerRequest<int, int_<42> >
            Path
            <
                PerRequest<I1, Impl1>,
                C1
            >
        >
    { };

    //injector
    //order in Ctor is not important
    Utility::Injector<BaseModule, BOOST_TYPEOF(fusionModule), GenericModule> injector(
        l_module,
        make_shared<Attr<std::string, mpl::string<'Name'> >(new std::string("MyString")),
        make_shared<I4>(new ExternalImpl4())
    );

    shared_ptr<C2> l_sp = injector.create< shared_ptr<C2> >();
    C2 l_value = injector.create<C2>();
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

