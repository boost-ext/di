//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/fusion_module.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

#include "boost/di/concepts.hpp"
#include "boost/di/named.hpp"
#include "fake_dependency.hpp"
#include "contains_all.hpp"
#include "data.hpp"

namespace boost {
namespace di {

struct value
{
    explicit value(int i)
        : i(i)
    { }

    int i;
};

struct dummy_scope
{
    template<typename>
    struct scope
    { };
};

BOOST_AUTO_TEST_CASE(empty) {
    BOOST_AUTO(module, fusion_module<>()());
    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector0<>
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((contains_all<mpl::vector0<>, module_t::pool::externals>::value));
}

BOOST_AUTO_TEST_CASE(default_scope_bind) {
    BOOST_AUTO(module, fusion_module<>()(
          bind<if0, c0if0>()
        , bind<c2>::in_name<int>()
        , bind<c3>::in_call<call_stack<c4, c5> >()));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::per_request, if0, c0if0>::type
              , fake_dependency_base_of<scopes::per_request, named<c2, int>, c2>::type
              , fake_dependency_base_of<scopes::per_request, c3, c3, call_stack<c4, c5> >::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                bind<if0, c0if0>
              , bind<c2>::in_name<int>
              , bind<c3>::in_call<call_stack<c4, c5> >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope) {
    BOOST_AUTO(module, fusion_module<>()(
        scope<scopes::singleton>::bind<
            c0if0
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton, c0if0, c0if0>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                scope<scopes::singleton>::bind<
                    c0if0
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_alias) {
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            c0if0
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton, c0if0, c0if0>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                singletons<
                    c0if0
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_direct) {
    BOOST_AUTO(module, fusion_module<>()(
        singleton<c0if0>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton, c0if0, c0if0>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                singleton<c0if0>
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(custom_scope) {
    BOOST_AUTO(module, fusion_module<>()(
        scope<dummy_scope>::bind<c0if0>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<dummy_scope, c0if0, c0if0>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                scope<dummy_scope>::bind<c0if0>
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(many_singletons) {
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            c1, c2, c3
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton, c1, c1>::type
              , fake_dependency_base_of<scopes::singleton, c2, c2>::type
              , fake_dependency_base_of<scopes::singleton, c3, c3>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                singletons<
                    c1, c2, c3
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(many_scopes) {
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
          c1, c2
        >()
      , per_requests<
          c3, c4
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton, c1, c1>::type
              , fake_dependency_base_of<scopes::singleton, c2, c2>::type
              , fake_dependency_base_of<scopes::per_request, c3, c3>::type
              , fake_dependency_base_of<scopes::per_request, c4, c4>::type
            >,
            module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                singletons<
                  c1, c2
                >
              , per_requests<
                  c3, c4
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_call) {
    BOOST_AUTO(module, fusion_module<>()(
        per_request<c1>::in_call<c2>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::per_request, c1, c1, c2>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                per_request<c1>::in_call<c2>
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_name) {
    BOOST_AUTO(module, fusion_module<>()(
        singleton<c1>::in_name<int>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton, named<c1, int>, c1>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                singleton<c1>::in_name<int>
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_namein_call) {
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<c1>::in_name<int>::in_call<double>
          , bind<c2>::in_name<double>::in_call<int>
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton, named<c1, int>, c1, double>::type
              , fake_dependency_base_of<scopes::singleton, named<c2, double>, c2, int>::type
            >,
            module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                singletons<
                    bind<c1>::in_name<int>::in_call<double>
                  , bind<c2>::in_name<double>::in_call<int>
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_call_in_name) {
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<c1>::in_call<double>::in_name<int>
          , bind<c2>::in_call<int>::in_name<double>
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton, named<c1, int>, c1, double>::type
              , fake_dependency_base_of<scopes::singleton, named<c2, double>, c2, int>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                singletons<
                    bind<c1>::in_call<double>::in_name<int>
                  , bind<c2>::in_call<int>::in_name<double>
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_if) {
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<if0, c0if0>
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::singleton, if0, c0if0>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                singletons<
                    bind<if0, c0if0>
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(mix) {
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<if0, c0if0>
          , c1
          , bind<c2>::in_name<int>
          , bind<c3>::in_call<call_stack<c4, c5> >
        >()
      , per_requests<
            c6
        >()
      , singleton<c7>::in_name<double>::in_call<c1>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::singleton, if0, c0if0>::type
              , fake_dependency_base_of<scopes::singleton, c1, c1>::type
              , fake_dependency_base_of<scopes::singleton, named<c2, int>, c2>::type
              , fake_dependency_base_of<scopes::singleton, c3, c3, call_stack<c4, c5> >::type
              , fake_dependency_base_of<scopes::per_request, c6, c6>::type
              , fake_dependency_base_of<scopes::singleton, named<c7, double>, c7, c1>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                singletons<
                    bind<if0, c0if0>
                  , c1
                  , bind<c2>::in_name<int>
                  , bind<c3>::in_call<call_stack<c4, c5> >
                >
              , per_requests<
                    c6
                >
              , singleton<c7>::in_name<double>::in_call<c1>
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_in_call) {
    BOOST_AUTO(module, fusion_module<>()(
        per_requests<
            bind<int, mpl::int_<1> >
          , bind<int, mpl::int_<4> >::in_name<mpl::string<'2'> >::in_call<call_stack<c7, c6, c4> >
          , bind<int, mpl::int_<5> >::in_call<c2>
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::per_request, int, mpl::int_<1> >::type
              , fake_dependency_base_of<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<4>, call_stack<c7, c6, c4> >::type
              , fake_dependency_base_of<scopes::per_request, int, mpl::int_<5>, c2>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                per_requests<
                    bind<int, mpl::int_<1> >
                  , bind<int, mpl::int_<4> >::in_name<mpl::string<'2'> >::in_call<call_stack<c7, c6, c4> >
                  , bind<int, mpl::int_<5> >::in_call<c2>
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(multiple_calls) {
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<c0>::in_call<c1, call_stack<c2, c3>, c4 >
        >()
      , bind<c5>::in_call<int, double>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton, c0, c0, c1, call_stack<c2, c3>, c4>::type
              , fake_dependency_base_of<scopes::per_request, c5, c5, int, double>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                singletons<
                    bind<c0>::in_call<c1, call_stack<c2, c3>, c4 >
                >
              , bind<c5>::in_call<int, double>
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(to_basic) {
    const int i = 42;
    const int d = 87.0;

    BOOST_AUTO(module, fusion_module<>()(
        bind<int>::to(i)
      , bind<double>::to(d)
    ));

    c14 c14_ = module.create<c14>();

    BOOST_CHECK_EQUAL(i, c14_.i);
    BOOST_CHECK_EQUAL(d, c14_.d);
}

BOOST_AUTO_TEST_CASE(to_in_name) {
    const int i1 = 42;
    const int i2 = 87;

    BOOST_AUTO(module, fusion_module<>()(
        bind<int>::in_name<mpl::string<'1'> >::to(i1)
      , bind<int>::in_name<mpl::string<'2'> >::to(i2)
    ));

    c10 c10_ = module.create<c10>();

    BOOST_CHECK_EQUAL(i1, c10_.i1);
    BOOST_CHECK_EQUAL(i2, c10_.i2);
}

BOOST_AUTO_TEST_CASE(to_in_call) {
    const int i = 42;

    BOOST_AUTO(module, fusion_module<>()(
        bind<int>::in_call<c3>::to(i)
    ));

    c4 c4_ = module.create<c4>();

    BOOST_CHECK_EQUAL(i, c4_.c3_->i);
    BOOST_CHECK_EQUAL(0, c4_.i1);
    BOOST_CHECK_EQUAL(0, c4_.i2);
}

BOOST_AUTO_TEST_CASE(to_in_call_in_name) {
    const int i1 = 42;
    const int i2 = 87;

    BOOST_AUTO(module, fusion_module<>()(
        bind<int>::in_call<c4>::in_name<mpl::string<'1'> >::to(i1)
      , bind<int>::in_call<c4>::in_name<mpl::string<'2'> >::to(i2)
      , bind<c0if0>()
    ));

    c6 c6_ = module.create<c6>();

    BOOST_CHECK_EQUAL(i1, c6_.c4_->i1);
    BOOST_CHECK_EQUAL(i2, c6_.c4_->i2);
}

BOOST_AUTO_TEST_CASE(to_in_name_in_call) {
    const int i1 = 42;
    const int i2 = 87;

    BOOST_AUTO(module, fusion_module<>()(
        bind<int>::in_name<mpl::string<'1'> >::in_call<c4>::to(i1)
      , bind<int>::in_name<mpl::string<'2'> >::in_call<c4>::to(i2)
      , bind<c0if0>()
    ));

    c6 c6_ = module.create<c6>();

    BOOST_CHECK_EQUAL(i1, c6_.c4_->i1);
    BOOST_CHECK_EQUAL(i2, c6_.c4_->i2);
}

BOOST_AUTO_TEST_CASE(to_in_call_stack) {
    const int i = 42;

    BOOST_AUTO(module, fusion_module<>()(
        bind<int>::in_call<call_stack<c4, c3> >::to(i)
      , bind<c0if0>()
    ));

    c6 c6_ = module.create<c6>();

    BOOST_CHECK_EQUAL(i, c6_.c4_->c3_->i);
    BOOST_CHECK_EQUAL(0, c6_.c4_->i1);
    BOOST_CHECK_EQUAL(0, c6_.c4_->i2);
}

BOOST_AUTO_TEST_CASE(to_variant_shared_ptr) {
    shared_ptr<c3> c3_(new c3);

    BOOST_AUTO(module, fusion_module<>()(
        bind<c3>::to(c3_)
    ));

    c4 c4_ = module.create<c4>();

    BOOST_CHECK_EQUAL(c3_, c4_.c3_);
}

BOOST_AUTO_TEST_CASE(to_variant_ref) {
    //const int i = 42;
    //const double d = 87.0;
    //c3 c3_(i);
    //c14 c14_(i, d);

    //const c3& c3_const_ref = c3_;
    //c14& c14_ref = c14_;

    //BOOST_AUTO(module, fusion_module<>()(
        //bind<c3>::to(c3_const_ref)
      //, bind<c14>::to(c14_ref)
    //));

    //c16 c16_ = module.create<c16>();

    //BOOST_CHECK_EQUAL(i, c16_.c3_.i);
    //BOOST_CHECK_EQUAL(i, c16_.c14_.i);
    //BOOST_CHECK_EQUAL(d, c16_.c14_.d);
}

BOOST_AUTO_TEST_CASE(create) {
}

BOOST_AUTO_TEST_CASE(visit) {
}

BOOST_AUTO_TEST_CASE(call) {
}

} // namespace di
} // namespace boost

