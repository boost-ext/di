QDeps (C++ Dependency Injection Framework)
================================

QDeps is a dependency injection framework.

To get started: git clone --recursive git://github.com/QSrc/QDeps.git

Requirements
------------
    Code:
        * C++ 98 standard-compliant compiler (e.g. >= gcc 3.4)
        * Boost >= 1.43 (headers only libraries)

    Tests:
        * GNU-compatible Make >= 3.81
        * gtest >= 1.6.0 (UT, MT)

Tests
------------
    cd Test/UT && make test

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

    struct SimpleModule : Front::Generic::Module
    {
    public:
        typedef vector
        <
            Impl<I1, Impl1>,                                                    //per request
            Impl<I2, Impl2>,                                                    //create I2 using Impl2

            Scope<Singleton>::Bind                                              //one instantion
            <
                Impl<I3, Impl3>,
                Inst<I4>,                                                       //external inst
                Inst<Attr<int, string<'Port'> >, int_<5> >,                     //set to 5
                Inst<Attr<std::string, mpl::string<'Name'> > >                  //external value
            >,

            Impl<I1, ImplI11>::Bind<C2>                                         //custom bind I1 to C2
            Inst<int, int_<42> >::Bind<C4, CallStack<C1, C2> >                  //bind int=42 to C4 and C1->C2
        >
        Binding;
    };

    //order in Ctor is not important
    Utility::Injector<SimpleModule> injector(
        make_shared<Attr<std::string, mpl::string<'Name'> >(new std::string("MyString")),
        make_shared<I4>(new ExternalImpl4())
    );

    shared_ptr<C2> l_sp = injector.create< shared_ptr<C2> >();
    const C2& l_cref = injector.create<const C2&>();
```

Author
------
Krzysztof Jusiak (krzysztof at jusiak dot net)

Author
------
    * Method injector
    * Providers
    * Xml front end
    * C++11 fork

License
-------
Boost Software License - Version 1.0 - August 17th, 2003

