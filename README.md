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

    #include <QDeps/Front/Ctor.hpp>
    #include <QDeps/Front/Generic/Module.hpp>
    #include <QDeps/Utitlity/Injector.hpp>

    using namespace QDeps;

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

    //fusion front end
    BOOST_AUTO(l_module, (make_shared<Front::Fusion::Module()(
        Bind<I>::To<Impl>(),
        Inst<int>(new int(32)),
        Bind<I1>::To<Im1>::InScope<Singleton>::InCall<C1, C2>()
    ));

    //generic front end
    struct SimpleModule : Front::Generic::Module
    {
    public:
        typedef vector
        <
            Impl<I1, Impl1>,                                        //per request
            Impl<I2, Impl2>,                                        //create I2 using Impl2
            Impl<vector<Q1, Q2>, ImplQ>,                            //assign ImplQ to Q1 and Q2

            Scope<Singleton>::Bind                                  //one instantion
            <
                Impl<I3, Impl3>,
                Inst<I4>,                                           //external inst
                Inst<Attr<int, string<'Port'> >, int_<5> >,         //set to 5
                Inst<Attr<std::string, mpl::string<'Name'> > >      //external value
            >,

            Impl<I1, ImplI11>::Bind<C2>                             //custom bind I1 to C2
            Inst<int, int_<42> >::Bind<C4, CallStack<C1, C2> >      //bind int=42 to C4 and C1->C2
        >
        Binding;
    };

    //injector
    //order in Ctor is not important
    Utility::Injector<SimpleModule, BOOST_TYPEOF(l_module)> injector(
        l_module,
        make_shared<Attr<std::string, mpl::string<'Name'> >(new std::string("MyString")),
        make_shared<I4>(new ExternalImpl4())
    );

    shared_ptr<C2> l_sp = injector.create< shared_ptr<C2> >();
    const C2& l_cref = injector.create<const C2&>();
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

