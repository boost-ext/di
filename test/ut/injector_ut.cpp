//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/injector.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/unique.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/concepts.hpp"

#include "common/fakes/fake_dependency.hpp"
#include "common/fakes/fake_visitor.hpp"
#include "common/fakes/fake_scope.hpp"
#include "common/contains_all.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {

struct double_value {
    static double value;
};
double double_value::value = 0;

class module {
    using injector_t = injector<bind<if0, c0if0>>;

public:
    injector_t configure() const {
        return injector_t();
    }
};

BOOST_AUTO_TEST_CASE(create) {
    struct injector_type
        : injector<
              unique<
                  c0if0
              >
          >
    { } injector_;

    auto c8_ = injector_.create<aux::shared_ptr<c8>>();

    BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(0, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(visit) {
    struct injector_type
        : injector<
              unique<
                  transaction_provider
                , mpl::int_<0>
              >
          >
    { } injector_;

    fake_visitor<
        mpl::vector<
            transaction_usage
          , aux::shared_ptr<provider<aux::shared_ptr<transaction>>>
          , aux::shared_ptr<c3>
          , int
        >
    > visitor;

    injector_.visit<transaction_usage>(visitor);
}

BOOST_AUTO_TEST_CASE(call) {
    fake_scope<>::entry_calls() = 0;
    fake_scope<>::exit_calls() = 0;

    struct injector_type
        : injector<
              scope<fake_scope<>>::bind<c0if0>
          >
    { } injector_;

    BOOST_CHECK_EQUAL(0, fake_scope<>::entry_calls());
    BOOST_CHECK_EQUAL(0, fake_scope<>::exit_calls());

    injector_.call(fake_scope_entry());
    BOOST_CHECK_EQUAL(1, fake_scope<>::entry_calls());
    BOOST_CHECK_EQUAL(0, fake_scope<>::exit_calls());

    injector_.call(fake_scope_exit());
    BOOST_CHECK_EQUAL(1, fake_scope<>::entry_calls());
    BOOST_CHECK_EQUAL(1, fake_scope<>::exit_calls());
}

BOOST_AUTO_TEST_CASE(empty) {
    struct injector_type
       : injector<>
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector0<>
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(default_scope_deduce) {
    struct injector_type
        : injector<c1>
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::deduce, c1, c1>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(default_scope_many) {
    struct injector_type
        : injector<c1, c2, c3>
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::deduce, c1, c1>::type
              , fake_dependency<scopes::deduce, c2, c2>::type
              , fake_dependency<scopes::deduce, c3, c3>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(default_scope_bind) {
    struct injector_type
        : injector<
              bind<if0, c0if0>
            , c1
            , bind<c2>::named<int>
            , bind<c3>::when<call_stack<c4, c5>>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::deduce, if0, c0if0>::type
              , fake_dependency<scopes::deduce, c1, c1>::type
              , fake_dependency<scopes::deduce, c2, c2, int>::type
              , fake_dependency<scopes::deduce, c3, c3, no_name, mpl::vector<call_stack<c4, c5>>>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(custom_scope) {
    struct injector_type
        : injector<
              scope<fake_scope<>>::bind<
                  c0if0
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<fake_scope<>, c0if0, c0if0>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope) {
    struct injector_type
        : injector<
              scope<scopes::shared<>>::bind<
                  c0if0
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c0if0, c0if0>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_alias) {
    struct injector_type
        : injector<
              shared<
                  c0if0
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c0if0, c0if0>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_direct) {
    struct injector_type
        : injector<
              shared<c0if0>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c0if0, c0if0>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(many_shared) {
    struct injector_type
        : injector<
              shared<
                c1, c2, c3
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c1, c1>::type
              , fake_dependency<scopes::shared<>, c2, c2>::type
              , fake_dependency<scopes::shared<>, c3, c3>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(many_scopes) {
    struct injector_type
        : injector<
              shared<
                  c1, c2
              >
            , unique<
                  c3, c4
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c1, c1>::type
              , fake_dependency<scopes::shared<>, c2, c2>::type
              , fake_dependency<scopes::unique<>, c3, c3>::type
              , fake_dependency<scopes::unique<>, c4, c4>::type
            >,
            injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(when) {
    struct injector_type
        : injector<
              unique<bind<c1>::when<call_stack<c2>>>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::unique<>, c1, c1, no_name, mpl::vector<call_stack<c2>>>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_type) {
    struct injector_type
        : injector<
              shared<bind<c1>::named<int>>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c1, c1, int>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_context) {
    struct injector_type
        : injector<
              shared<
                  bind<c1>::named<int>::when<call_stack<double>>
                , bind<c2>::named<double>::when<call_stack<int>>
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c1, c1, int, mpl::vector<call_stack<double>>>::type
              , fake_dependency<scopes::shared<>, c2, c2, double, mpl::vector<call_stack<int>>>::type
            >,
            injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(when_named) {
    struct injector_type
        : injector<
              shared<
                  bind<c1>::when<call_stack<double>>::named<int>
                , bind<c2>::when<call_stack<int>>::named<double>
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c1, c1, int, mpl::vector<call_stack<double>>>::type
              , fake_dependency<scopes::shared<>, c2, c2, double, mpl::vector<call_stack<int>>>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_if) {
    struct injector_type
        : injector<
              shared<
                  bind<if0, c0if0>
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, if0, c0if0>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(mix) {
    struct injector_type
        : injector<
              shared<
                  bind<if0, c0if0>
                , c1
                , bind<c2>::named<int>
                , bind<c3>::when<call_stack<c4, c5>>
              >
            , unique<
                c6
              >
            , shared<bind<c7>::named<double>::when<call_stack<c1>>>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, if0, c0if0>::type
              , fake_dependency<scopes::shared<>, c1, c1>::type
              , fake_dependency<scopes::shared<>, c2, c2, int>::type
              , fake_dependency<scopes::shared<>, c3, c3, no_name, mpl::vector<call_stack<c4, c5>>>::type
              , fake_dependency<scopes::unique<>, c6, c6>::type
              , fake_dependency<scopes::shared<>, c7, c7, double, mpl::vector<call_stack<c1>>>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_when_complex) {
    struct injector_type
        : injector<
              unique<
                  bind<int, mpl::int_<1>>
                , bind<int, mpl::int_<4>>::named<mpl::string<'2'>>::when<call_stack<c7, c6, c4>>
                , bind<int, mpl::int_<5>>::when<call_stack<c2>>
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::unique<>, int, mpl::int_<1>>::type
              , fake_dependency<scopes::unique<>, int, mpl::int_<4>, mpl::string<'2'>, mpl::vector<call_stack<c7, c6, c4>>>::type
              , fake_dependency<scopes::unique<>, int, mpl::int_<5>, no_name, mpl::vector<call_stack<c2>>>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(multiple_calls) {
    struct injector_type
        : injector<
              shared<
                  bind<c0>::when<call_stack<c1>, call_stack<c2, c3>, call_stack<c4>>
              >
            , bind<c5>::when<call_stack<int>, call_stack<double>>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c0, c0, no_name, mpl::vector<call_stack<c1>, call_stack<c2, c3>, call_stack<c4>>>::type
              , fake_dependency<scopes::deduce, c5, c5, no_name, mpl::vector<call_stack<int>, call_stack<double>>>::type
            >
          , injector_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(create_injector) {
    auto injector_ = injector<>()(
        unique<
            c0if0
        >()
    );

    auto c8_ = injector_.create<aux::shared_ptr<c8>>();

    BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(0, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(visit_injector) {
    auto injector_ = injector<>()(
        unique<
            transaction_provider
          , mpl::int_<0>
        >()
    );

    fake_visitor<
        mpl::vector<
            transaction_usage
          , aux::shared_ptr<provider<aux::shared_ptr<transaction>>>
          , aux::shared_ptr<c3>
          , int
        >
    > visitor;

    injector_.visit<transaction_usage>(visitor);
}

BOOST_AUTO_TEST_CASE(call_injector) {
    fake_scope<>::entry_calls() = 0;
    fake_scope<>::exit_calls() = 0;

    auto injector_ = injector<>()(
        scope<fake_scope<>>::bind<c0if0>()
    );

    BOOST_CHECK_EQUAL(0, fake_scope<>::entry_calls());
    BOOST_CHECK_EQUAL(0, fake_scope<>::exit_calls());

    injector_.call(fake_scope_entry());
    BOOST_CHECK_EQUAL(1, fake_scope<>::entry_calls());
    BOOST_CHECK_EQUAL(0, fake_scope<>::exit_calls());

    injector_.call(fake_scope_exit());
    BOOST_CHECK_EQUAL(1, fake_scope<>::entry_calls());
    BOOST_CHECK_EQUAL(1, fake_scope<>::exit_calls());
}

BOOST_AUTO_TEST_CASE(empty_injector) {
    auto injector_ = injector<>()();
    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector0<>
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(default_scope_bind_injector) {
    auto injector_ = injector<>()(
          bind<if0, c0if0>()
        , bind<c2>::named<int>()
        , bind<c3>::when<call_stack<c4, c5>>()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::deduce, if0, c0if0>::type
              , fake_dependency<scopes::deduce, c2, c2, int>::type
              , fake_dependency<scopes::deduce, c3, c3, no_name, mpl::vector<call_stack<c4, c5>>>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_injector) {
    auto injector_ = injector<>()(
        scope<scopes::shared<>>::bind<
            c0if0
        >()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c0if0, c0if0>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_alias_injector) {
    auto injector_ = injector<>()(
        shared<
            c0if0
        >()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c0if0, c0if0>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_direct_injector) {
    auto injector_ = injector<>()(
        shared<c0if0>()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c0if0, c0if0>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(custom_scope_injector) {
    auto injector_ = injector<>()(
        scope<fake_scope<>>::bind<c0if0>()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<fake_scope<>, c0if0, c0if0>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(many_shared_injector) {
    auto injector_ = injector<>()(
        shared<
            c1, c2, c3
        >()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c1, c1>::type
              , fake_dependency<scopes::shared<>, c2, c2>::type
              , fake_dependency<scopes::shared<>, c3, c3>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(many_scopes_injector) {
    auto injector_ = injector<>()(
        shared<
          c1, c2
        >()
      , unique<
          c3, c4
        >()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c1, c1>::type
              , fake_dependency<scopes::shared<>, c2, c2>::type
              , fake_dependency<scopes::unique<>, c3, c3>::type
              , fake_dependency<scopes::unique<>, c4, c4>::type
            >,
            injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(when_injector) {
    auto injector_ = injector<>()(
        unique<bind<c1>::when<call_stack<c2>>>()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::unique<>, c1, c1, no_name, mpl::vector<call_stack<c2>>>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_injector) {
    auto injector_ = injector<>()(
        shared<bind<c1>::named<int>>()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c1, c1, int>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_when) {
    auto injector_ = injector<>()(
        shared<
            bind<c1>::named<int>::when<call_stack<double>>
          , bind<c2>::named<double>::when<call_stack<int>>
        >()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c1, c1, int, mpl::vector<call_stack<double>>>::type
              , fake_dependency<scopes::shared<>, c2, c2, double, mpl::vector<call_stack<int>>>::type
            >,
            injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(when_named_injector) {
    auto injector_ = injector<>()(
        shared<
            bind<c1>::when<call_stack<double>>::named<int>
          , bind<c2>::when<call_stack<int>>::named<double>
        >()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c1, c1, int, mpl::vector<call_stack<double>>>::type
              , fake_dependency<scopes::shared<>, c2, c2, double, mpl::vector<call_stack<int>>>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_if_injector) {
    auto injector_ = injector<>()(
        shared<
            bind<if0, c0if0>
        >()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, if0, c0if0>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(mix_injector) {
    auto injector_ = injector<>()(
        shared<
            bind<if0, c0if0>
          , c1
          , bind<c2>::named<int>
          , bind<c3>::when<call_stack<c4, c5>>
        >()
      , unique<
            c6
        >()
      , shared<bind<c7>::named<double>::when<call_stack<c1>>>()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, if0, c0if0>::type
              , fake_dependency<scopes::shared<>, c1, c1>::type
              , fake_dependency<scopes::shared<>, c2, c2, int>::type
              , fake_dependency<scopes::shared<>, c3, c3, no_name, mpl::vector<call_stack<c4, c5>>>::type
              , fake_dependency<scopes::unique<>, c6, c6>::type
              , fake_dependency<scopes::shared<>, c7, c7, double, mpl::vector<call_stack<c1>>>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_when_injector) {
    auto injector_ = injector<>()(
        unique<
            bind<int, mpl::int_<1>>
          , bind<int, mpl::int_<4>>::named<mpl::string<'2'>>::when<call_stack<c7, c6, c4>>
          , bind<int, mpl::int_<5>>::when<call_stack<c2>>
        >()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::unique<>, int, mpl::int_<1>>::type
              , fake_dependency<scopes::unique<>, int, mpl::int_<4>, mpl::string<'2'>, mpl::vector<call_stack<c7, c6, c4>>>::type
              , fake_dependency<scopes::unique<>, int, mpl::int_<5>, no_name, mpl::vector<call_stack<c2>>>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(multiple_calls_injector) {
    auto injector_ = injector<>()(
        shared<
            bind<c0>::when<call_stack<c1>, call_stack<c2, c3>, call_stack<c4>>
        >()
      , bind<c5>::when<call_stack<int>, call_stack<double>>()
    );

    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::shared<>, c0, c0, no_name, mpl::vector<call_stack<c1>, call_stack<c2, c3>, call_stack<c4>>>::type
              , fake_dependency<scopes::deduce, c5, c5, no_name, mpl::vector<call_stack<int>, call_stack<double>>>::type
            >
          , injector_t::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(to_basic) {
    const int i = 42;
    const double d = 87.0;

    auto injector_ = injector<>()(
        bind<int>::to(i)
      , bind<double>::to(d)
    );

    auto c14_ = injector_.create<c14>();

    BOOST_CHECK_EQUAL(i, c14_.i);
    BOOST_CHECK_EQUAL(d, c14_.d);
}

BOOST_AUTO_TEST_CASE(to_named) {
    const int i1 = 42;
    const int i2 = 87;

    auto injector_ = injector<>()(
        bind<int>::named<mpl::string<'1'>>::to(i1)
      , bind<int>::named<mpl::string<'2'>>::to(i2)
    );

    auto c10_ = injector_.create<c10>();

    BOOST_CHECK_EQUAL(i1, c10_.i1);
    BOOST_CHECK_EQUAL(i2, c10_.i2);
}

BOOST_AUTO_TEST_CASE(to_when) {
    const int i = 42;

    auto injector_ = injector<>()(
        bind<int>::when<call_stack<c3>>::to(i)
    );

    auto c4_ = injector_.create<c4>();

    BOOST_CHECK_EQUAL(i, c4_.c3_->i);
    BOOST_CHECK_EQUAL(0, c4_.i1);
    BOOST_CHECK_EQUAL(0, c4_.i2);
}

BOOST_AUTO_TEST_CASE(to_when_named) {
    const int i1 = 42;
    const int i2 = 87;

    auto injector_ = injector<>()(
        bind<int>::when<call_stack<c4>>::named<mpl::string<'1'>>::to(i1)
      , bind<int>::when<call_stack<c4>>::named<mpl::string<'2'>>::to(i2)
      , bind<c0if0>()
    );

    auto c6_ = injector_.create<c6>();

    BOOST_CHECK_EQUAL(i1, c6_.c4_->i1);
    BOOST_CHECK_EQUAL(i2, c6_.c4_->i2);
}

BOOST_AUTO_TEST_CASE(to_named_when) {
    const int i1 = 42;
    const int i2 = 87;

    auto injector_ = injector<>()(
        bind<int>::named<mpl::string<'1'>>::when<call_stack<c4>>::to(i1)
      , bind<int>::named<mpl::string<'2'>>::when<call_stack<c4>>::to(i2)
      , bind<c0if0>()
    );

    auto c6_ = injector_.create<c6>();

    BOOST_CHECK_EQUAL(i1, c6_.c4_->i1);
    BOOST_CHECK_EQUAL(i2, c6_.c4_->i2);
}

BOOST_AUTO_TEST_CASE(to_when_with_global) {
    const int i1 = 42;
    const int i2 = 87;

    auto injector_ = injector<>()(
        bind<int>::when<call_stack<c3>>::to(i1)
      , bind<int>::to(i2)
    );

    auto c4_ = injector_.create<c4>();

    BOOST_CHECK_EQUAL(i1, c4_.c3_->i);
    BOOST_CHECK_EQUAL(87, c4_.i1);
    BOOST_CHECK_EQUAL(87, c4_.i2);
}

BOOST_AUTO_TEST_CASE(to_when_stack) {
    const int i = 42;

    auto injector_ = injector<>()(
        unique<
            c3
        >()
      , bind<int>::when<call_stack<c4, c3>>::to(i)
      , bind<c0if0>()
    );

    auto c6_ = injector_.create<c6>();

    BOOST_CHECK_EQUAL(42, c6_.c4_->c3_->i);
    BOOST_CHECK_EQUAL(0, c6_.c4_->i1);
    BOOST_CHECK_EQUAL(0, c6_.c4_->i2);
}

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    aux::shared_ptr<c3> c3_(new c3);

    auto injector_ = injector<>()(
        bind<c3>::to(c3_)
    );

    auto c4_ = injector_.create<c4>();

    BOOST_CHECK_EQUAL(c3_, c4_.c3_);
}

BOOST_AUTO_TEST_CASE(to_ref) {
    const int i = 42;
    const double d = 87.0;
    c3 c3_(i);
    c14 c14_(i, d);

    const c3& c3_const_ref = c3_;
    c14& c14_ref = c14_;

    auto injector_ = injector<>()(
        bind<c3>::to(cref(c3_const_ref))
      , bind<c14>::to(ref(c14_ref))
    );

    auto c16_ = injector_.create<aux::shared_ptr<c16>>();

    BOOST_CHECK(&c3_const_ref == &c16_->c3_);
    BOOST_CHECK(&c14_ref == &c16_->c14_);

    BOOST_CHECK_EQUAL(c3_.i, c16_->c3_.i);
    BOOST_CHECK_EQUAL(c14_.i, c16_->c14_.i);
    BOOST_CHECK_EQUAL(c14_.d, c16_->c14_.d);
}

BOOST_AUTO_TEST_CASE(to_ref_no_copy) {
    const int i = 42;
    const double d = 87.0;
    c3 c3_(i);
    c14 c14_(i, d);

    auto injector_ = injector<>()(
        bind<c3>::to(cref(c3_))
      , bind<c14>::to(ref(c14_))
    );

    auto c16_ = injector_.create<aux::shared_ptr<c16>>();

    BOOST_CHECK(&c3_ == &c16_->c3_);
    BOOST_CHECK(&c14_ == &c16_->c14_);

    BOOST_CHECK_EQUAL(c3_.i, c16_->c3_.i);
    BOOST_CHECK_EQUAL(c14_.i, c16_->c14_.i);
    BOOST_CHECK_EQUAL(c14_.d, c16_->c14_.d);
}

BOOST_AUTO_TEST_CASE(fixed_value) {
    double_value::value = 42.0;

    auto injector_ = injector<>()(
        bind<double, double_value>()
    );

    BOOST_CHECK_EQUAL(double_value::value, injector_.create<double>());
}

BOOST_AUTO_TEST_CASE(modules) {
    auto injector_ = injector<>()(module());
    using injector_t = decltype(injector_);

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::deduce, if0, c0if0>::type
            >
          , injector_t::deps
        >::value
    ));

    BOOST_CHECK(dynamic_cast<c0if0*>(injector_.create<aux::auto_ptr<if0>>().get()));
}

} // namespace di
} // namespace boot

