//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>
#include "test/common/data.hpp"
#include "test/common/visitor.hpp"
#include "di/di.hpp"

#define BOOST_AUTO_TEST_CASE_VARIADIC(test, type, ...) \
    typedef boost::mpl::vector<__VA_ARGS__> test##types; BOOST_AUTO_TEST_CASE_TEMPLATE(test, type, test##types)

namespace di
{
namespace mt
{

using namespace boost;
using namespace boost::mpl;
using namespace test::common;
using namespace utility;
using namespace front::base;
using namespace back;

typedef front::base::module <
    singletons <
        c3
    >,
    per_requests <
        cif0,
        bind<cif01>::in_call_stack<c6, c5>,
        bind<cif02>::in_call_stack<c7>,
        bind<int, int_<1> >,
        bind<int, int_<2> >::in_call_stack<c8>,
        bind<int, int_<3> >::in_name< mpl::string<'1'> >::in_call_stack<c7, c6, c4>,
        bind<int, int_<4> >::in_name< mpl::string<'2'> >::in_call_stack<c7, c6, c4>,
        bind<int, int_<5> >::in_call_stack<c2>
    >
> basemodule1;

typedef front::base::module <
    singletons <
        c3
    >,
    per_requests <
        bind<int, int_<0> >::in_name< mpl::string<'1'> >,
        bind<int, int_<1> >
    >
> basemodule2;

typedef front::base::module <
    singletons <
        cif0
    >,
    per_requests <
        bind<int, int_<2> >::in_call_stack<c8>,
        bind<int, int_<3> >::in_name< mpl::string<'2'> >
    >
> basemodule3;

typedef front::base::module <
    per_requests <
        transaction_provider, int_<0>
    >
> provider_module;

typedef front::base::module <
    singletons <
        cif0
    >,
    externals <
        int,
        double
    >
> externalsmodule;

struct externalsmodulector : front::base::module <
    singletons <
        cif0
    >,
    externals <
        int,
        double
    >
>
{
    externalsmodulector(int i, double d)
        : module(Set<int>(i), Set<double>(d))
    { }
};

BOOST_AUTO(fusionmodule1, front::fusion::module<>()(
    singletons <
        c3
    >(),
    per_requests <
        cif0,
        bind<cif01>::in_call_stack<c6, c5>,
        bind<cif02>::in_call_stack<c7>,
        bind<int, int_<1> >,
        bind<int, int_<2> >::in_call_stack<c8>,
        bind<int, int_<3> >::in_name< mpl::string<'1'> >::in_call_stack<c7, c6, c4>,
        bind<int, int_<4> >::in_name< mpl::string<'2'> >::in_call_stack<c7, c6, c4>,
        bind<int, int_<5> >::in_call_stack<c2>
    >()
));

BOOST_AUTO(fusionmodule2, front::fusion::module<>()(
    singletons <
        cif0
    >(),
    per_requests <
        bind<int, int_<2> >::in_call_stack<c8>,
        bind<int, int_<3> >::in_name< mpl::string<'2'> >
    >()
));

BOOST_AUTO(fusionmodule3, front::fusion::module<>()(
    singletons <
        c3
    >(),
    per_requests <
        bind<int, int_<0> >::in_name< mpl::string<'1'> >,
        bind<int, int_<1> >
    >()
));

BOOST_AUTO(fusion_provider_module, front::fusion::module<>()(
    per_requests <
        transaction_provider, int_<0>
    >()
));

BOOST_AUTO_TEST_CASE_VARIADIC(Onemodule, injector,
    injector<basemodule1>,
    injector<BOOST_TYPEOF(fusionmodule1)>)
{
    injector inj;

    shared_ptr<c8> c8 = inj.template create< shared_ptr<c8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 != c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<cif01*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<cif02*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(2, c8->i);
    BOOST_CHECK_EQUAL(3, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(4, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(5, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(Manymodules, injector,
    injector<basemodule2, basemodule3>,
    injector<basemodule3, basemodule2>,
    injector<BOOST_TYPEOF(fusionmodule2), BOOST_TYPEOF(fusionmodule3)>,
    injector<BOOST_TYPEOF(fusionmodule3), BOOST_TYPEOF(fusionmodule2)>)
{
    injector inj;

    shared_ptr<c8> c8 = inj.template create< shared_ptr<c8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 == c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<cif0*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<cif0*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(2, c8->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(3, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(Mixmodules, injector,
    injector<basemodule2, BOOST_TYPEOF(fusionmodule2)>,
    injector<BOOST_TYPEOF(fusionmodule2), basemodule2>)
{
    injector inj;

    shared_ptr<c8> c8 = inj.template create< shared_ptr<c8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 == c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<cif0*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<cif0*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(2, c8->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(3, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(Basicprovider, injector,
    injector<provider_module>,
    injector<BOOST_TYPEOF(fusion_provider_module)>)
{
    injector inj;
    transaction_usage obj = inj.template create<transaction_usage>();
    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

BOOST_AUTO_TEST_CASE_VARIADIC(Basicvisitor, injector,
    injector<provider_module>,
    injector<BOOST_TYPEOF(fusion_provider_module)>)
{
    injector inj;
    visitor
    <
        vector
        <
            transaction_usage,
            shared_ptr< provider< shared_ptr<transaction> > >,
            shared_ptr<c3>,
            int
        >
    >
    visitor;

    inj.template visit<transaction_usage>(visitor);
}

BOOST_AUTO_TEST_CASE_VARIADIC(Basicexternals, injector,
    injector<externalsmodule>)
{
    injector inj(
        externalsmodule(
            externalsmodule::Set<int>(42),
            externalsmodule::Set<double>(87.0)
        )
    );

    shared_ptr<c9> c9 = inj.template create< shared_ptr<c9> >();

    BOOST_CHECK_EQUAL(42, c9->i);
    BOOST_CHECK_EQUAL(87.0, c9->d);
}

BOOST_AUTO_TEST_CASE_VARIADIC(Basicexternalsctor, injector,
    injector<externalsmodulector>)
{
    injector inj(
        externalsmodulector(42, 87.0)
    );

    shared_ptr<c9> c9 = inj.template create< shared_ptr<c9> >();

    BOOST_CHECK_EQUAL(42, c9->i);
    BOOST_CHECK_EQUAL(87.0, c9->d);
}

} // namespace mt
} // namespace di

