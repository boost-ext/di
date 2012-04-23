//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>
#include "boost/di.hpp"
#include "visitor.hpp"
#include "data.hpp"

#define BOOST_AUTO_TEST_CASE_VARIADIC(test, type, ...) \
    typedef mpl::vector<__VA_ARGS__> test##types; BOOST_AUTO_TEST_CASE_TEMPLATE(test, type, test##types)

namespace boost {
namespace di {

typedef generic_module<
    singletons<
        c3
    >
  , per_requests<
        c0if0
      , bind<c1if0>::in_call<c6, c5>
      , bind<c2if0>::in_call<c7>
      , bind<int, mpl::int_<1> >
      , bind<int, mpl::int_<2> >::in_call<c8>
      , bind<int, mpl::int_<3> >::in_name< mpl::string<'1'> >::in_call<c7, c6, c4>
      , bind<int, mpl::int_<4> >::in_name< mpl::string<'2'> >::in_call<c7, c6, c4>
      , bind<int, mpl::int_<5> >::in_call<c2>
    >
> base_module_1;

typedef generic_module<
    singletons<
        c3
    >
  , per_requests<
        bind<int, mpl::int_<0> >::in_name< mpl::string<'1'> >
      , bind<int, mpl::int_<1> >
    >
> base_module_2;

typedef generic_module<
    singletons<
        c0if0
    >
  , per_requests<
        bind<int, mpl::int_<2> >::in_call<c8>
      , bind<int, mpl::int_<3> >::in_name< mpl::string<'2'> >
    >
> base_module_3;

typedef generic_module<
    per_requests<
        transaction_provider
      , mpl::int_<0>
    >
> provider_module;

typedef generic_module<
    singletons<
        c0if0
    >
  , externals<
        int
      , double
    >
> externals_module;

struct externals_module_ctor : generic_module<
    singletons<
        c0if0
    >
  , externals<
        int
      , double
    >
>
{
    externals_module_ctor(int i, double d)
        : generic_module(set<int>(i), set<double>(d))
    { }
};

BOOST_AUTO(fusion_module_1, fusion_module<>()(
    singletons<
        c3
    >()
  , per_requests<
        c0if0
      , bind<c1if0>::in_call<c6, c5>
      , bind<c2if0>::in_call<c7>
      , bind<int, mpl::int_<1> >
      , bind<int, mpl::int_<2> >::in_call<c8>
      , bind<int, mpl::int_<3> >::in_name< mpl::string<'1'> >::in_call<c7, c6, c4>
      , bind<int, mpl::int_<4> >::in_name< mpl::string<'2'> >::in_call<c7, c6, c4>
      , bind<int, mpl::int_<5> >::in_call<c2>
    >()
));

BOOST_AUTO(fusion_module_2, fusion_module<>()(
    singletons<
        c0if0
    >()
  , per_requests<
        bind<int, mpl::int_<2> >::in_call<c8>
      , bind<int, mpl::int_<3> >::in_name< mpl::string<'2'> >
    >()
));

BOOST_AUTO(fusion_module_3, fusion_module<>()(
    singletons<
        c3
    >()
  , per_requests<
        bind<int, mpl::int_<0> >::in_name< mpl::string<'1'> >
      , bind<int, mpl::int_<1> >
    >()
));

BOOST_AUTO(fusion_provider_module, fusion_module<>()(
    per_requests<
        transaction_provider, mpl::int_<0>
    >()
));

BOOST_AUTO_TEST_CASE_VARIADIC(one_module, TInjector,
    injector<base_module_1>,
    injector<BOOST_TYPEOF(fusion_module_1)>)
{
    TInjector injector;

    shared_ptr<c8> c8_ = injector.template create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c2if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(2, c8_->i);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(4, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(5, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(many_modules, TInjector,
    injector<base_module_2, base_module_3>,
    injector<base_module_3, base_module_2>,
    injector<BOOST_TYPEOF(fusion_module_2), BOOST_TYPEOF(fusion_module_3)>,
    injector<BOOST_TYPEOF(fusion_module_3), BOOST_TYPEOF(fusion_module_2)>)
{
    TInjector injector;

    shared_ptr<c8> c8_ = injector.template create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ == c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(2, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(mix_modules, TInjector,
    injector<base_module_2, BOOST_TYPEOF(fusion_module_2)>,
    injector<BOOST_TYPEOF(fusion_module_2), base_module_2>)
{
    TInjector injector;

    shared_ptr<c8> c8_ = injector.template create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ == c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(2, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE_VARIADIC(basic_provider, TInjector,
    injector<provider_module>,
    injector<BOOST_TYPEOF(fusion_provider_module)>)
{
    TInjector injector;
    transaction_usage obj = injector.template create<transaction_usage>();
    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

BOOST_AUTO_TEST_CASE_VARIADIC(Basicvisitor, TInjector,
    injector<provider_module>,
    injector<BOOST_TYPEOF(fusion_provider_module)>)
{
    TInjector injector;

    visitor<
        mpl::vector<
            transaction_usage
          , shared_ptr< provider< shared_ptr<transaction> > >
          , shared_ptr<c3>
          , int
        >
    > visitor_mock;

    injector.template visit<transaction_usage>(visitor_mock);
}

BOOST_AUTO_TEST_CASE_VARIADIC(basic_externals, TInjector,
    injector<externals_module>)
{
    TInjector injector(
        externals_module(
            externals_module::set<int>(42)
          , externals_module::set<double>(87.0)
        )
    );

    shared_ptr<c9> c9_ = injector.template create< shared_ptr<c9> >();

    BOOST_CHECK_EQUAL(42, c9_->i);
    BOOST_CHECK_EQUAL(87.0, c9_->d);
}

BOOST_AUTO_TEST_CASE_VARIADIC(basic_externals_ctor, TInjector,
    injector<externals_module_ctor>)
{
    TInjector injector(
        externals_module_ctor(42, 87.0)
    );

    shared_ptr<c9> c9_ = injector.template create< shared_ptr<c9> >();

    BOOST_CHECK_EQUAL(42, c9_->i);
    BOOST_CHECK_EQUAL(87.0, c9_->d);
}

} // namespace di
} // namespace boost

