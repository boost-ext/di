//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/fusion_module.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

#include "boost/di/aux_/dependency.hpp"
#include "boost/di/concepts.hpp"
#include "boost/di/named.hpp"
#include "dependency.hpp"
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

BOOST_AUTO_TEST_CASE(fusion_module_empty)
{
    BOOST_AUTO(module, fusion_module<>()());
    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector0<>
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module_t::pool::externals>::value));
}

BOOST_AUTO_TEST_CASE(fusion_module_default_scope_bind)
{
    BOOST_AUTO(module, fusion_module<>()(
          bind<if0, c0if0>()
        , bind<c2>::in_name<int>()
        , bind<c3>::in_call<call_stack<c4, c5> >()));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency<scopes::per_request, if0, c0if0>::type
              , dependency_base_of<scopes::per_request, named<c2, int>, c2>::type
              , dependency_base_of<scopes::per_request, c3, c3, call_stack<c4, c5> >::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                bind<if0, c0if0>
              , bind<c2>::in_name<int>
              , bind<c3>::in_call<call_stack<c4, c5> >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(fusion_module_one_scope)
{
    BOOST_AUTO(module, fusion_module<>()(
        scope<scopes::singleton>::bind<
            c0if0
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c0if0, c0if0>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                scope<scopes::singleton>::bind<
                    c0if0
                >
            >
          , module_t::pool::externals
        >::value
    ));

}

BOOST_AUTO_TEST_CASE(fusion_module_one_scope_alias)
{
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            c0if0
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c0if0, c0if0>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                singletons<
                    c0if0
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(fusion_module_one_scope_direct)
{
    BOOST_AUTO(module, fusion_module<>()(
        singleton<c0if0>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c0if0, c0if0>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                singleton<c0if0>
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(fusion_module_many_singletons)
{
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            c1, c2, c3
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c1, c1>::type
              , dependency_base_of<scopes::singleton, c2, c2>::type
              , dependency_base_of<scopes::singleton, c3, c3>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                singletons<
                    c1, c2, c3
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(fusion_module_many_scopes)
{
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
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c1, c1>::type
              , dependency_base_of<scopes::singleton, c2, c2>::type
              , dependency_base_of<scopes::per_request, c3, c3>::type
              , dependency_base_of<scopes::per_request, c4, c4>::type
            >,
            module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
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

BOOST_AUTO_TEST_CASE(fusion_module_in_call)
{
    BOOST_AUTO(module, fusion_module<>()(
        per_request<c1>::in_call<c2>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::per_request, c1, c1, c2>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                per_request<c1>::in_call<c2>
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(fusion_module_in_name)
{
    BOOST_AUTO(module, fusion_module<>()(
        singleton<c1>::in_name<int>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, named<c1, int>, c1>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                singleton<c1>::in_name<int>
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(fusion_module_in_namein_call)
{
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<c1>::in_name<int>::in_call<double>
          , bind<c2>::in_name<double>::in_call<int>
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, named<c1, int>, c1, double>::type
              , dependency_base_of<scopes::singleton, named<c2, double>, c2, int>::type
            >,
            module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
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

BOOST_AUTO_TEST_CASE(fusion_module_in_call_in_name)
{
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<c1>::in_call<double>::in_name<int>
          , bind<c2>::in_call<int>::in_name<double>
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, named<c1, int>, c1, double>::type
              , dependency_base_of<scopes::singleton, named<c2, double>, c2, int>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
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

BOOST_AUTO_TEST_CASE(fusion_module_bind_if)
{
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<if0, c0if0>
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency<scopes::singleton, if0, c0if0>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                singletons<
                    bind<if0, c0if0>
                >
            >
          , module_t::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(fusion_module_mix)
{
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
        mpl::equal<
            mpl::vector<
                dependency<scopes::singleton, if0, c0if0>::type
              , dependency_base_of<scopes::singleton, c1, c1>::type
              , dependency_base_of<scopes::singleton, named<c2, int>, c2>::type
              , dependency_base_of<scopes::singleton, c3, c3, call_stack<c4, c5> >::type
              , dependency_base_of<scopes::per_request, c6, c6>::type
              , dependency_base_of<scopes::singleton, named<c7, double>, c7, c1>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
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

BOOST_AUTO_TEST_CASE(fusion_module_named_in_call)
{
    BOOST_AUTO(module, fusion_module<>()(
        per_requests<
            bind<int, mpl::int_<1> >
          , bind<int, mpl::int_<4> >::in_name<mpl::string<'2'> >::in_call<call_stack<c7, c6, c4> >
          , bind<int, mpl::int_<5> >::in_call<c2>
        >()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::per_request, int, mpl::int_<1> >::type
              , dependency_base_of<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<4>, call_stack<c7, c6, c4> >::type
              , dependency_base_of<scopes::per_request, int, mpl::int_<5>, c2>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
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

BOOST_AUTO_TEST_CASE(fusion_multiple_calls)
{
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<c0>::in_call<c1, call_stack<c2, c3>, c4 >
        >()
      , bind<c5>::in_call<int, double>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c0, c0, c1, call_stack<c2, c3>, c4>::type
              , dependency_base_of<scopes::per_request, c5, c5, int, double>::type
            >
          , module_t::deps
        >::value
    ));

    BOOST_CHECK((
        mpl::equal<
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

} // namespace di
} // namespace boost

