Quick Pool (QPool)
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
    #include <QDeps/Front/Binding/Module.hpp>
    #include <QDeps/Utitlity/Injector.hpp>

    using namespace QDeps;

    class C1
    {
    public:
        QDPES_CTOR(Ctor, boost::shared_ptr<I1> p1, boost::shared_ptr<I2> p2)
            : m1(p1), m2(p2)
        { }

    private:
        boost::shared_ptr<I1> m1;
        boost::shared_ptr<I2> m2;
    };

    class SimpleModule
    {
    public:
        typedef boost::mpl::vector
        <
            Impl<I1, Impl1>,                            //per request
            Impl<I2, Impl2>,                            //create I2 using Impl2

            Scope<Singleton>::Bind                      //one instantion
            <
                Impl<I3, Impl3>,
                Inst<I4>,                               //external inst
                Inst<Attr<int, 'Port' >, 5>,            //set to 5
                Inst<Attr<std::string, 'Name' >, _1>    //external value
            >,

            Impl<I1, ImplI11>::Bind<C1>                 //custom bind I1 to C1
        >
        Binding;
    };

    //order of Ctor is not important
    Utility::Injector< Binding::Module<SimpleModule> > injector(
        boost::make_shared<Inst< Attr<std::string, 'Name'>, _1>(new int("MyString")),
        boost::make_shared<Inst<I4> >(new ExternalImpl4())
    );

    boost::shared_ptr<C2> l_sp = injector.create< boost::shared_ptr<C2> >();
    const C2& l_cref = injector.create<const C2&>();
```

Author
------
Krzysztof Jusiak (krzysztof at jusiak dot net)

License
-------
Boost Software License - Version 1.0 - August 17th, 2003

