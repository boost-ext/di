//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "boost/di/aux/instance.hpp"
#include "boost/di/aux/dependency.hpp"
#include "boost/di/named.hpp"
#include "boost/di/generic_module.hpp"
#include "boost/di/concepts.hpp"
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

BOOST_AUTO_TEST_CASE(generic_module_empty)
{
    struct module
       : generic_module<>
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector0<>
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_default_scope)
{
    struct module
        : generic_module<c1>
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::per_request, c1, c1>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_default_scope_many)
{
    struct module
        : generic_module<c1, c2, c3>
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::per_request, c1, c1>::type
              , dependency_base_of<scopes::per_request, c2, c2>::type
              , dependency_base_of<scopes::per_request, c3, c3>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_default_scope_bind)
{
    struct module
        : generic_module<
              bind<if0, c0if0>
            , c1
            , bind<c2>::in_name<int>
            , bind<c3>::in_call<call_stack<c4, c5> >
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency<scopes::per_request, if0, c0if0>::type
              , dependency_base_of<scopes::per_request, c1, c1>::type
              , dependency_base_of<scopes::per_request, named<c2, int>, c2>::type
              , dependency_base_of<scopes::per_request, c3, c3, call_stack<c4, c5> >::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_one_scope)
{
    struct module
        : generic_module<
              scope<scopes::singleton>::bind<
                  c0if0
              >
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c0if0, c0if0>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_one_scope_alias)
{
    struct module
        : generic_module<
              singletons<
                  c0if0
              >
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c0if0, c0if0>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_one_scope_direct)
{
    struct module
        : generic_module<
              singleton<c0if0>
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c0if0, c0if0>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_many_singletons)
{
    struct module
        : generic_module<
              singletons<
                c1, c2, c3
              >
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c1, c1>::type
              , dependency_base_of<scopes::singleton, c2, c2>::type
              , dependency_base_of<scopes::singleton, c3, c3>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_many_scopes)
{
    struct module
        : generic_module<
              singletons<
                  c1, c2
              >
            , per_requests<
                  c3, c4
              >
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c1, c1>::type
              , dependency_base_of<scopes::singleton, c2, c2>::type
              , dependency_base_of<scopes::per_request, c3, c3>::type
              , dependency_base_of<scopes::per_request, c4, c4>::type
            >,
            module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_in_call)
{
    struct module
        : generic_module<
              per_request<c1>::in_call<c2>
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::per_request, c1, c1, c2>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_in_name)
{
    struct module
        : generic_module<
              singleton<c1>::in_name<int>
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, named<c1, int>, c1>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_in_namein_call)
{
    struct module
        : generic_module<
              singletons<
                  bind<c1>::in_name<int>::in_call<double>
                , bind<c2>::in_name<double>::in_call<int>
              >
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, named<c1, int>, c1, double>::type
              , dependency_base_of<scopes::singleton, named<c2, double>, c2, int>::type
            >,
            module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_in_call_in_name)
{
    struct module
        : generic_module<
              singletons<
                  bind<c1>::in_call<double>::in_name<int>
                , bind<c2>::in_call<int>::in_name<double>
              >
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, named<c1, int>, c1, double>::type
              , dependency_base_of<scopes::singleton, named<c2, double>, c2, int>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_bind_if)
{
    struct module
        : generic_module<
              singletons<
                  bind<if0, c0if0>
              >
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency<scopes::singleton, if0, c0if0>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_mix)
{
    struct module
        : generic_module<
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
    { };

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
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_named_in_call)
{
    struct module
        : generic_module<
              per_requests<
                  bind<int, mpl::int_<1> >
                , bind<int, mpl::int_<4> >::in_name<mpl::string<'2'> >::in_call<call_stack<c7, c6, c4> >
                , bind<int, mpl::int_<5> >::in_call<c2>
              >
          >
    { };

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::per_request, int, mpl::int_<1> >::type
              , dependency_base_of<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<4>, call_stack<c7, c6, c4> >::type
              , dependency_base_of<scopes::per_request, int, mpl::int_<5>, c2>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(generic_multiple_calls)
{
    struct module
        : generic_module<
              singletons<
                  bind<c0>::in_call<c1, call_stack<c2, c3>, c4 >
              >
            , bind<c5>::in_call<int, double>
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency_base_of<scopes::singleton, c0, c0, c1, call_stack<c2, c3>, c4>::type
              , dependency_base_of<scopes::per_request, c5, c5, int, double>::type
            >
          , module::deps
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_externals_base)
{
    struct module
        : generic_module<
              externals<
                  c1
              >
          >
    { };

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::deps>::value));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                aux::instance<c1>
            >
          , module::pool::sequence
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(generic_module_externals_mix)
{
    struct module
        : generic_module<
              externals<
                  c1, c2
              >
            , external<c3>
          >
    { };

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::deps>::value));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                aux::instance<c1>
              , aux::instance<c2>
              , aux::instance<c3>
            >
          , module::pool::sequence
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(generic_module_externals_bind)
{
    struct module
        : generic_module<
              externals<
                  int
                , bind<c1>::in_name<int>
                , bind<c2>::in_call<c1>
                , bind<c3>::in_name<double>::in_call<c4, c5>
              >
          >
    { };

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::deps>::value));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                aux::instance<int>
              , aux::instance<named<c1, int> >
              , aux::instance<c2, mpl::vector<c1> >
              , aux::instance<named<c3, double>, mpl::vector<c4, c5> >
            >
          , module::pool::sequence
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(generic_module_set_instance_int)
{
    const int i = 42;

    struct module
        : generic_module<
              externals<
                  int
              >
          >
    { };

    BOOST_CHECK_EQUAL(i, module::set<int>(i).get());
}

BOOST_AUTO_TEST_CASE(generic_module_set_instance_annotate_in_in_call)
{
    const int i1 = 42;
    const int i2 = 43;

    struct module
        : generic_module<
              externals<
                  annotate<bind<int>::in_call<c1, c2> >::with<a>
                , annotate<bind<int>::in_call<c3, c4> >::with<b>
              >
          >
    { };

    BOOST_CHECK_EQUAL(i1, module::set<a>(i1).get());
    BOOST_CHECK_EQUAL(i2, module::set<b>(i2).get());
}

BOOST_AUTO_TEST_CASE(generic_module_set_instance_annotate_in_name)
{
    const int i1 = 42;
    const int i2 = 43;

    struct module
        : generic_module<
              externals<
                  annotate<bind<int>::in_name<float> >::with<a>
                , annotate<bind<int>::in_name<double> >::with<b>
              >
          >
    { };

    BOOST_CHECK_EQUAL(i1, module::set<a>(i1).get());
    BOOST_CHECK_EQUAL(i2, module::set<b>(i2).get());
}

BOOST_AUTO_TEST_CASE(generic_module_set_instance_annotatein_name_in_call)
{
    const int i1 = 42;
    const int i2 = 43;

    struct module
        : generic_module<
              externals<
                  annotate<bind<int>::in_call<c1, c2>::in_name<float> >::with<a>
                , annotate<bind<int>::in_name<double>::in_call<c3, c4> >::with<b>
              >
          >
    { };

    BOOST_CHECK_EQUAL(i1, module::set<a>(i1).get());
    BOOST_CHECK_EQUAL(i2, module::set<b>(i2).get());
}

BOOST_AUTO_TEST_CASE(generic_module_set_instance_mix)
{
    const int i1 = 42;
    const int i2 = 43;
    const int i3 = 44;

    struct module
        : generic_module<
              externals<
                  int
                , annotate<bind<int>::in_name<float> >::with<a>
                , annotate<bind<int>::in_call<c1, c2>::in_name<float> >::with<b>
              >
          >
    { };

    BOOST_CHECK_EQUAL(i1, module::set<int>(i1).get());
    BOOST_CHECK_EQUAL(i2, module::set<a>(i2).get());
    BOOST_CHECK_EQUAL(i3, module::set<b>(i3).get());
}

BOOST_AUTO_TEST_CASE(generic_module_ctor_with_externals)
{
    const int i = 42;
    const double d = 87.0;

    typedef generic_module<
        externals<
            int
          , double
        >
    > module;

    module module_(
        module::set<int>(i),
        module::set<double>(d)
    );

    BOOST_CHECK_EQUAL(i, module_.get_pool().get<aux::instance<int> >());
    BOOST_CHECK_EQUAL(d, module_.get_pool().get<aux::instance<double> >());
}

BOOST_AUTO_TEST_CASE(generic_module_ctor_with_externals_shared_ptr)
{
    const int i = 42;
    shared_ptr<value> v(new value(i));

    typedef generic_module<
        external<
            value
        >
    > module;

    module module_(
        module::set<value>(v)
    );

    BOOST_CHECK_EQUAL(
        i
      , get<shared_ptr<value> >(module_.get_pool().get<aux::instance<value> >())->i
    );
}

} // namespace di
} // namespace boot

