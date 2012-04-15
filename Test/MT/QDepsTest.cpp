//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>
#include "Test/Common/Data.hpp"
#include "Test/Common/Visitor.hpp"
#include "QDeps/QDeps.hpp"

#define BOOST_AUTO_TEST_CASE_VARIADIC(test, type, ...) \
    typedef boost::mpl::vector<__VA_ARGS__> test##types; BOOST_AUTO_TEST_CASE_TEMPLATE(test, type, test##types)

namespace QDeps
{
namespace MT
{

using namespace boost;
using namespace boost::mpl;
using namespace Test::Common;
using namespace Utility;
using namespace Front::Base;
using namespace Back;

typedef Front::Base::Module <
    Singletons <
        C3
    >,
    PerRequests <
        CIf0,
        Bind<CIf01>::InCallStack<C6, C5>,
        Bind<CIf02>::InCallStack<C7>,
        Bind<int, int_<1> >,
        Bind<int, int_<2> >::InCallStack<C8>,
        Bind<int, int_<3> >::InName< mpl::string<'1'> >::InCallStack<C7, C6, C4>,
        Bind<int, int_<4> >::InName< mpl::string<'2'> >::InCallStack<C7, C6, C4>,
        Bind<int, int_<5> >::InCallStack<C2>
    >
> BaseModule1;

typedef Front::Base::Module <
    Singletons <
        C3
    >,
    PerRequests <
        Bind<int, int_<0> >::InName< mpl::string<'1'> >,
        Bind<int, int_<1> >
    >
> BaseModule2;

typedef Front::Base::Module <
    Singletons <
        CIf0
    >,
    PerRequests <
        Bind<int, int_<2> >::InCallStack<C8>,
        Bind<int, int_<3> >::InName< mpl::string<'2'> >
    >
> BaseModule3;

typedef Front::Base::Module <
    PerRequests <
        TransactionProvider, int_<0>
    >
> ProviderModule;

typedef Front::Base::Module <
    Singletons <
        CIf0
    >,
    Externals <
        int,
        double
    >
> ExternalsModule;

struct ExternalsModuleCtor : Front::Base::Module <
    Singletons <
        CIf0
    >,
    Externals <
        int,
        double
    >
>
{
    ExternalsModuleCtor(int i, double d)
        : Module(Set<int>(i), Set<double>(d))
    { }
};

BOOST_AUTO(fusionModule1, Front::Fusion::Module<>()(
    Singletons <
        C3
    >(),
    PerRequests <
        CIf0,
        Bind<CIf01>::InCallStack<C6, C5>,
        Bind<CIf02>::InCallStack<C7>,
        Bind<int, int_<1> >,
        Bind<int, int_<2> >::InCallStack<C8>,
        Bind<int, int_<3> >::InName< mpl::string<'1'> >::InCallStack<C7, C6, C4>,
        Bind<int, int_<4> >::InName< mpl::string<'2'> >::InCallStack<C7, C6, C4>,
        Bind<int, int_<5> >::InCallStack<C2>
    >()
));

BOOST_AUTO(fusionModule2, Front::Fusion::Module<>()(
    Singletons <
        CIf0
    >(),
    PerRequests <
        Bind<int, int_<2> >::InCallStack<C8>,
        Bind<int, int_<3> >::InName< mpl::string<'2'> >
    >()
));

BOOST_AUTO(fusionModule3, Front::Fusion::Module<>()(
    Singletons <
        C3
    >(),
    PerRequests <
        Bind<int, int_<0> >::InName< mpl::string<'1'> >,
        Bind<int, int_<1> >
    >()
));

BOOST_AUTO(providerModule, Front::Fusion::Module<>()(
    PerRequests <
        TransactionProvider, int_<0>
    >()
));

BOOST_AUTO_TEST_CASE_VARIADIC(OneModule, Injector,
    Injector<BaseModule1>,
    Injector<BOOST_TYPEOF(fusionModule1)>)
{
    Injector injector;

    shared_ptr<C8> c8 = injector.template create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 != c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf01*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf02*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(2, c8->i);
    BOOST_CHECK_EQUAL(3, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(4, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(5, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(ManyModules, Injector,
    Injector<BaseModule2, BaseModule3>,
    Injector<BaseModule3, BaseModule2>,
    Injector<BOOST_TYPEOF(fusionModule2), BOOST_TYPEOF(fusionModule3)>,
    Injector<BOOST_TYPEOF(fusionModule3), BOOST_TYPEOF(fusionModule2)>)
{
    Injector injector;

    shared_ptr<C8> c8 = injector.template create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 == c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(2, c8->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(3, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(MixModules, Injector,
    Injector<BaseModule2, BOOST_TYPEOF(fusionModule2)>,
    Injector<BOOST_TYPEOF(fusionModule2), BaseModule2>)
{
    Injector injector;

    shared_ptr<C8> c8 = injector.template create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 == c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(2, c8->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(3, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(BasicProvider, Injector,
    Injector<ProviderModule>,
    Injector<BOOST_TYPEOF(providerModule)>)
{
    Injector injector;
    TransactionUsage obj = injector.template create<TransactionUsage>();
    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

BOOST_AUTO_TEST_CASE_VARIADIC(BasicVisitor, Injector,
    Injector<ProviderModule>,
    Injector<BOOST_TYPEOF(providerModule)>)
{
    Injector injector;
    Visitor
    <
        vector
        <
            TransactionUsage,
            shared_ptr< Provider< shared_ptr<Transaction> > >,
            shared_ptr<C3>,
            int
        >
    >
    visitor;

    injector.template visit<TransactionUsage>(visitor);
}

BOOST_AUTO_TEST_CASE_VARIADIC(BasicExternals, Injector,
    Injector<ExternalsModule>)
{
    Injector injector(
        ExternalsModule(
            ExternalsModule::Set<int>(42),
            ExternalsModule::Set<double>(87.0)
        )
    );

    shared_ptr<C9> c9 = injector.template create< shared_ptr<C9> >();

    BOOST_CHECK_EQUAL(42, c9->i);
    BOOST_CHECK_EQUAL(87.0, c9->d);
}

BOOST_AUTO_TEST_CASE_VARIADIC(BasicExternalsCtor, Injector,
    Injector<ExternalsModuleCtor>)
{
    Injector injector(
        ExternalsModuleCtor(42, 87.0)
    );

    shared_ptr<C9> c9 = injector.template create< shared_ptr<C9> >();

    BOOST_CHECK_EQUAL(42, c9->i);
    BOOST_CHECK_EQUAL(87.0, c9->d);
}

} // namespace MT
} // namespace QDeps

