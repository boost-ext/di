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
#include <boost/mpl/or.hpp>
#include "boost/di/aux/instance.hpp"
#include "boost/di/aux/dependency.hpp"
#include "boost/di/named.hpp"
#include "boost/di/generic_module.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace test {

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
          , module::dependencies
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
                aux::dependency<back::scopes::per_request, c1, c1, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c1>, is_same<mpl::_1, c1> > >
            >
          , module::dependencies
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
                aux::dependency<back::scopes::per_request, c1, c1, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c1>, is_same<mpl::_1, c1> > >
              , aux::dependency<back::scopes::per_request, c2, c2, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c2>, is_same<mpl::_1, c2> > >
              , aux::dependency<back::scopes::per_request, c3, c3, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c3>, is_same<mpl::_1, c3> > >
            >
          , module::dependencies
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
            , bind<c3>::in_call_stack<c4, c5>
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                aux::dependency<back::scopes::per_request, if0, c0if0, mpl::vector0<>, is_same<mpl::_1, if0> >
              , aux::dependency<back::scopes::per_request, c1, c1, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c1>, is_same<mpl::_1, c1> > >
              , aux::dependency<back::scopes::per_request, named<c2, int>, c2, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, named<c2, int> >, is_same<mpl::_1, named<c2, int> > > >
              , aux::dependency<back::scopes::per_request, c3, c3, mpl::vector<c4, c5>, mpl::or_< is_base_of<mpl::_1, c3>, is_same<mpl::_1, c3> > >
            >
          , module::dependencies
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
                aux::dependency<back::scopes::singleton, c0if0, c0if0, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c0if0>, is_same<mpl::_1, c0if0> > >
            >
          , module::dependencies
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
                aux::dependency<back::scopes::singleton, c0if0, c0if0, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c0if0>, is_same<mpl::_1, c0if0> > >
            >
          , module::dependencies
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
                aux::dependency<back::scopes::singleton, c0if0, c0if0, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c0if0>, is_same<mpl::_1, c0if0> > >
            >
          , module::dependencies
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
                aux::dependency<back::scopes::singleton, c1, c1, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c1>, is_same<mpl::_1, c1> > >
              , aux::dependency<back::scopes::singleton, c2, c2, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c2>, is_same<mpl::_1, c2> > >
              , aux::dependency<back::scopes::singleton, c3, c3, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c3>, is_same<mpl::_1, c3> > >
            >
          , module::dependencies
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
                aux::dependency<back::scopes::singleton, c1, c1, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c1>, is_same<mpl::_1, c1> > >
              , aux::dependency<back::scopes::singleton, c2, c2, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c2>, is_same<mpl::_1, c2> > >
              , aux::dependency<back::scopes::per_request, c3, c3, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c3>, is_same<mpl::_1, c3> > >
              , aux::dependency<back::scopes::per_request, c4, c4, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c4>, is_same<mpl::_1, c4> > >
            >,
            module::dependencies
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_in_call_stack)
{
    struct module
        : generic_module<
              per_request<c1>::in_call_stack<c2>
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                aux::dependency<back::scopes::per_request, c1, c1, mpl::vector<c2>, mpl::or_< is_base_of<mpl::_1, c1>, is_same<mpl::_1, c1> > >
            >
          , module::dependencies
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
                aux::dependency<back::scopes::singleton, named<c1, int>, c1, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, named<c1, int> >, is_same<mpl::_1, named<c1, int> > > >
            >
          , module::dependencies
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_in_namein_call_stack)
{
    struct module
        : generic_module<
              singletons<
                  bind<c1>::in_name<int>::in_call_stack<double>
                , bind<c2>::in_name<double>::in_call_stack<int>
              >
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                aux::dependency<back::scopes::singleton, named<c1, int>, c1, mpl::vector<double>, mpl::or_< is_base_of<mpl::_1, named<c1, int> >, is_same<mpl::_1, named<c1, int> > > >
              , aux::dependency<back::scopes::singleton, named<c2, double>, c2, mpl::vector<int>, mpl::or_< is_base_of<mpl::_1,  named<c2, double> >, is_same<mpl::_1,  named<c2, double> > > >
            >,
            module::dependencies
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_in_call_stack_in_name)
{
    struct module
        : generic_module<
              singletons<
                  bind<c1>::in_call_stack<double>::in_name<int>
                , bind<c2>::in_call_stack<int>::in_name<double>
              >
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                aux::dependency<back::scopes::singleton, named<c1, int>, c1, mpl::vector<double>, mpl::or_< is_base_of<mpl::_1, named<c1, int> >, is_same<mpl::_1, named<c1, int> > > >
              , aux::dependency<back::scopes::singleton, named<c2, double>, c2, mpl::vector<int>, mpl::or_< is_base_of<mpl::_1,  named<c2, double> >, is_same<mpl::_1,  named<c2, double> > > >
            >
          , module::dependencies
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
                aux::dependency<back::scopes::singleton, if0, c0if0, mpl::vector0<>, is_same<mpl::_1, if0> >
            >
          , module::dependencies
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
                , bind<c3>::in_call_stack<c4, c5>
              >
            , per_requests <
                c6
              >
            , singleton<c7>::in_name<double>::in_call_stack<c1>
          >
    { };

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                aux::dependency<back::scopes::singleton, if0, c0if0, mpl::vector0<>, is_same<mpl::_1, if0> >
              , aux::dependency<back::scopes::singleton, c1, c1, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c1>, is_same<mpl::_1, c1> > >
              , aux::dependency<back::scopes::singleton, named<c2, int>, c2, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, named<c2, int> >, is_same<mpl::_1, named<c2, int> > > >
              , aux::dependency<back::scopes::singleton, c3, c3, mpl::vector<c4, c5>, mpl::or_< is_base_of<mpl::_1, c3>, is_same<mpl::_1, c3> > >
              , aux::dependency<back::scopes::per_request, c6, c6, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c6>, is_same<mpl::_1, c6> > >
              , aux::dependency<back::scopes::singleton, named<c7, double>, c7, mpl::vector<c1>, mpl::or_< is_base_of<mpl::_1, named<c7, double> >, is_same<mpl::_1, named<c7, double> > > >
            >
          , module::dependencies
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(generic_module_named_in_call_stack)
{
    struct module
        : generic_module<
              per_requests<
                  bind<int, mpl::int_<1> >
                , bind<int, mpl::int_<4> >::in_name< string<'2'> >::in_call_stack<c7, c6, c4>,
                , bind<int, mpl::int_<5> >::in_call_stack<c2>
              >
          >
    { };

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::pool::sequence>::value));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                aux::dependency<back::scopes::per_request, int, mpl::int_<1>, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, int>, is_same<mpl::_1, int> > >
              , aux::dependency<back::scopes::per_request, named<int, string<'2'> >, mpl::int_<4>, mpl::vector<c7, c6, c4>, mpl::or_< is_base_of<mpl::_1, named<int, string<'2'> > >, is_same<mpl::_1, named<int, string<'2'> > > > >
              , aux::dependency<back::scopes::per_request, int, mpl::int_<5>, mpl::vector<c2>, mpl::or_< is_base_of<mpl::_1, int>, is_same<mpl::_1, int> > >
            >
          , module::dependencies
        >::value
    ));
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

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::dependencies>::value));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                instance<c1>
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

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::dependencies>::value));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                instance<c1>
              , instance<c2>
              , instance<c3>
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
                , bind<c2>::in_call_stack<c1>
                , bind<c3>::in_name<double>::in_call_stack<c4, c5>
              >
          >
    { };

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module::dependencies>::value));

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                instance<int>
              , instance<named<c1, int> >
              , instance<c2, mpl::vector<c1> >
              , instance<named<c3, double>, mpl::vector<c4, c5> >
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

BOOST_AUTO_TEST_CASE(generic_module_set_instance_annotate_in_in_call_stack)
{
    const int i1 = 42;
    const int i2 = 43;

    struct module
        : generic_module<
              externals<
                  annotate< bind<int>::in_call_stack<c1, c2> >::with<a>
                , annotate< bind<int>::in_call_stack<c3, c4> >::with<b>
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
                  annotate< bind<int>::in_name<float> >::with<a>
                , annotate< bind<int>::in_name<double> >::with<b>
              >
          >
    { };

    BOOST_CHECK_EQUAL(i1, module::set<a>(i1).get());
    BOOST_CHECK_EQUAL(i2, module::set<b>(i2).get());
}

BOOST_AUTO_TEST_CASE(generic_module_set_instance_annotatein_name_in_call_stack)
{
    const int i1 = 42;
    const int i2 = 43;

    struct module
        : generic_module<
              externals<
                  annotate< bind<int>::in_call_stack<c1, c2>::in_name<float> >::with<a>
                , annotate< bind<int>::in_name<double>::in_call_stack<c3, c4> >::with<b>
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
                , annotate< bind<int>::in_name<float> >::with<a>
                , annotate< bind<int>::in_call_stack<c1, c2>::in_name<float> >::with<b>
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

    BOOST_CHECK_EQUAL(i, module_.get_pool().get< instance<int> >());
    BOOST_CHECK_EQUAL(d, module_.get_pool().get< instance<double> >());
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

    BOOST_CHECK_EQUAL(i, get<shared_ptr<value> >(module_.get_pool().get< instance<value> >())->i);
}

} // namespace test
} // namespace base
} // namespace front
} // namespace di

