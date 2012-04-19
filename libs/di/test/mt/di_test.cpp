//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>
#include "mt/visitor.hpp"
#include "boost/di.hpp"
#include "data.hpp"

#define BOOST_AUTO_TEST_CASE_VARIADIC(test, type, ...) \
    typedef mpl::vector<__VA_ARGS__> test##types; BOOST_AUTO_TEST_CASE_TEMPLATE(test, type, test##types)

namespace boost {
namespace di {
namespace test {
namespace mt {

typedef front::base::module <
    singletons <
        c3
    >,
    per_requests <
        c0if0,
        bind<c1if0>::in_call_stack<c6, c5>,
        bind<c2if0>::in_call_stack<c7>,
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
        c0if0
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
        c0if0
    >,
    externals <
        int,
        double
    >
> externalsmodule;

struct externalsmodulector : front::base::module <
    singletons <
        c0if0
    >,
    externals <
        int,
        double
    >
>
{
    externalsmodulector(int i, double d)
        : module(set<int>(i), set<double>(d))
    { }
};

BOOST_AUTO(fusionmodule1, front::fusion::module<>()(
    singletons <
        c3
    >(),
    per_requests <
        c0if0,
        bind<c1if0>::in_call_stack<c6, c5>,
        bind<c2if0>::in_call_stack<c7>,
        bind<int, int_<1> >,
        bind<int, int_<2> >::in_call_stack<c8>,
        bind<int, int_<3> >::in_name< mpl::string<'1'> >::in_call_stack<c7, c6, c4>,
        bind<int, int_<4> >::in_name< mpl::string<'2'> >::in_call_stack<c7, c6, c4>,
        bind<int, int_<5> >::in_call_stack<c2>
    >()
));

BOOST_AUTO(fusionmodule2, front::fusion::module<>()(
    singletons <
        c0if0
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

    shared_ptr<c8> c8_ = inj.template create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 != c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c2if0*>(c8_->c7_->if0.get()));

    BOOST_CHECK_EQUAL(2, c8_->i);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(4, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(5, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(Manymodules, injector,
    injector<basemodule2, basemodule3>,
    injector<basemodule3, basemodule2>,
    injector<BOOST_TYPEOF(fusionmodule2), BOOST_TYPEOF(fusionmodule3)>,
    injector<BOOST_TYPEOF(fusionmodule3), BOOST_TYPEOF(fusionmodule2)>)
{
    injector inj;

    shared_ptr<c8> c8_ = inj.template create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 == c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0.get()));

    BOOST_CHECK_EQUAL(2, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(Mixmodules, injector,
    injector<basemodule2, BOOST_TYPEOF(fusionmodule2)>,
    injector<BOOST_TYPEOF(fusionmodule2), basemodule2>)
{
    injector inj;

    shared_ptr<c8> c8_ = inj.template create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 == c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0.get()));

    BOOST_CHECK_EQUAL(2, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
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
            externalsmodule::set<int>(42),
            externalsmodule::set<double>(87.0)
        )
    );

    shared_ptr<c9> c9_ = inj.template create< shared_ptr<c9> >();

    BOOST_CHECK_EQUAL(42, c9_->i);
    BOOST_CHECK_EQUAL(87.0, c9_->d);
}

BOOST_AUTO_TEST_CASE_VARIADIC(Basicexternalsctor, injector,
    injector<externalsmodulector>)
{
    injector inj(
        externalsmodulector(42, 87.0)
    );

    shared_ptr<c9> c9_ = inj.template create< shared_ptr<c9> >();

    BOOST_CHECK_EQUAL(42, c9_->i);
    BOOST_CHECK_EQUAL(87.0, c9_->d);
}

} // namespace mt
} // namespace test
} // namespace di
} // namespace boost

