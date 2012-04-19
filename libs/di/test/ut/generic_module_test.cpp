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
#include "test/common/data.hpp"
#include "di/back/aux/instance.hpp"
#include "di/back/aux/dependency.hpp"
#include "di/front/base/module.hpp"
#include "di/utility/named.hpp"

namespace di
{
namespace front
{
namespace base
{
namespace ut
{


class A { };
class B { };

struct value
{
    explicit value(int i)
        : i(i)
    { }

    int i;
};

BOOST_AUTO_TEST_CASE(Empty)
{
    struct testmodule : module<> { };

    BOOST_CHECK((
        equal
        <
            vector0<>,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(defaultscope)
{
    struct testmodule : module<c1> { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::per_request, c1, c1, vector0<>, or_< is_base_of<_1, c1>, is_same<_1, c1> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(defaultscopeMany)
{
    struct testmodule : module<c1, c2, c3> { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::per_request, c1, c1, vector0<>, or_< is_base_of<_1, c1>, is_same<_1, c1> > >,
                dependency<back::scopes::per_request, c2, c2, vector0<>, or_< is_base_of<_1, c2>, is_same<_1, c2> > >,
                dependency<back::scopes::per_request, c3, c3, vector0<>, or_< is_base_of<_1, c3>, is_same<_1, c3> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(defaultscopebind)
{
    struct testmodule : module
        <
            bind<if0, c0if0>,
            c1,
            bind<c2>::in_name<int>,
            bind<c3>::in_call_stack<c4, c5>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::per_request, if0, c0if0, vector0<>, is_same<_1, if0> >,
                dependency<back::scopes::per_request, c1, c1, vector0<>, or_< is_base_of<_1, c1>, is_same<_1, c1> > >,
                dependency<back::scopes::per_request, named<c2, int>, c2, vector0<>, or_< is_base_of<_1, named<c2, int> >, is_same<_1, named<c2, int> > > >,
                dependency<back::scopes::per_request, c3, c3, vector<c4, c5>, or_< is_base_of<_1, c3>, is_same<_1, c3> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(Onescope)
{
    struct testmodule : module
        <
            scope<back::scopes::singleton>::bind <
                c0if0
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, c0if0, c0if0, vector0<>, or_< is_base_of<_1, c0if0>, is_same<_1, c0if0> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(OnescopeAlias)
{
    struct testmodule : module
        <
            singletons <
                c0if0
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, c0if0, c0if0, vector0<>, or_< is_base_of<_1, c0if0>, is_same<_1, c0if0> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(OnescopeDirect)
{
    struct testmodule : module
        <
            singleton<c0if0>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, c0if0, c0if0, vector0<>, or_< is_base_of<_1, c0if0>, is_same<_1, c0if0> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(Many)
{
    struct testmodule : module
        <
            singletons<
                c1,
                c2,
                c3
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, c1, c1, vector0<>, or_< is_base_of<_1, c1>, is_same<_1, c1> > >,
                dependency<back::scopes::singleton, c2, c2, vector0<>, or_< is_base_of<_1, c2>, is_same<_1, c2> > >,
                dependency<back::scopes::singleton, c3, c3, vector0<>, or_< is_base_of<_1, c3>, is_same<_1, c3> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(Manyscopes)
{
    struct testmodule : module
        <
            singletons<
                c1,
                c2
            >,
            per_requests<
                c3,
                c4
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, c1, c1, vector0<>, or_< is_base_of<_1, c1>, is_same<_1, c1> > >,
                dependency<back::scopes::singleton, c2, c2, vector0<>, or_< is_base_of<_1, c2>, is_same<_1, c2> > >,
                dependency<back::scopes::per_request, c3, c3, vector0<>, or_< is_base_of<_1, c3>, is_same<_1, c3> > >,
                dependency<back::scopes::per_request, c4, c4, vector0<>, or_< is_base_of<_1, c4>, is_same<_1, c4> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(in_call_stack)
{
    struct testmodule : module
        <
            per_request<c1>::in_call_stack<c2>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::per_request, c1, c1, vector<c2>, or_< is_base_of<_1, c1>, is_same<_1, c1> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(in_name)
{
    struct testmodule : module
        <
            singleton<c1>::in_name<int>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, named<c1, int>, c1, vector0<>, or_< is_base_of<_1, named<c1, int> >, is_same<_1, named<c1, int> > > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(in_namein_call_stack)
{
    struct testmodule : module
        <
            singletons<
                bind<c1>::in_name<int>::in_call_stack<double>,
                bind<c2>::in_name<double>::in_call_stack<int>
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, named<c1, int>, c1, vector<double>, or_< is_base_of<_1, named<c1, int> >, is_same<_1, named<c1, int> > > >,
                dependency<back::scopes::singleton, named<c2, double>, c2, vector<int>, or_< is_base_of<_1,  named<c2, double> >, is_same<_1,  named<c2, double> > > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(in_call_stackin_name)
{
    struct testmodule : module
        <
            singletons<
                bind<c1>::in_call_stack<double>::in_name<int>,
                bind<c2>::in_call_stack<int>::in_name<double>
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, named<c1, int>, c1, vector<double>, or_< is_base_of<_1, named<c1, int> >, is_same<_1, named<c1, int> > > >,
                dependency<back::scopes::singleton, named<c2, double>, c2, vector<int>, or_< is_base_of<_1,  named<c2, double> >, is_same<_1,  named<c2, double> > > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(bindIf)
{
    struct testmodule : module
        <
            singletons<
                bind<if0, c0if0>
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, if0, c0if0, vector0<>, is_same<_1, if0> >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(Mix)
{
    struct testmodule : module
        <
            singletons<
                bind<if0, c0if0>,
                c1,
                bind<c2>::in_name<int>,
                bind<c3>::in_call_stack<c4, c5>
            >,
            per_requests <
                c6
            >,
            singleton<c7>::in_name<double>::in_call_stack<c1>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, if0, c0if0, vector0<>, is_same<_1, if0> >,
                dependency<back::scopes::singleton, c1, c1, vector0<>, or_< is_base_of<_1, c1>, is_same<_1, c1> > >,
                dependency<back::scopes::singleton, named<c2, int>, c2, vector0<>, or_< is_base_of<_1, named<c2, int> >, is_same<_1, named<c2, int> > > >,
                dependency<back::scopes::singleton, c3, c3, vector<c4, c5>, or_< is_base_of<_1, c3>, is_same<_1, c3> > >,
                dependency<back::scopes::per_request, c6, c6, vector0<>, or_< is_base_of<_1, c6>, is_same<_1, c6> > >,
                dependency<back::scopes::singleton, named<c7, double>, c7, vector<c1>, or_< is_base_of<_1, named<c7, double> >, is_same<_1, named<c7, double> > > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(namedin_call_stack)
{
    struct testmodule : front::base::module
        <
            per_requests <
                bind<int, int_<1> >,
                bind<int, int_<4> >::in_name< string<'2'> >::in_call_stack<c7, c6, c4>,
                bind<int, int_<5> >::in_call_stack<c2>
            >
        >
    { };

    BOOST_CHECK((equal<vector0<>, testmodule::pool::sequence>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::per_request, int, int_<1>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
                dependency<back::scopes::per_request, named<int, string<'2'> >, int_<4>, vector<c7, c6, c4>, or_< is_base_of<_1, named<int, string<'2'> > >, is_same<_1, named<int, string<'2'> > > > >,
                dependency<back::scopes::per_request, int, int_<5>, vector<c2>, or_< is_base_of<_1, int>, is_same<_1, int> > >
            >,
            testmodule::dependencies
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(externalsbase)
{
    struct testmodule : module
        <
            externals<
                c1
            >
        >
    { };

    BOOST_CHECK((equal<vector0<>, testmodule::dependencies>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                instance<c1>
            >,
            testmodule::pool::sequence
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(externalsMix)
{
    struct testmodule : module
        <
            externals<
                c1,
                c2
            >,
            External<c3>
        >
    { };

    BOOST_CHECK((equal<vector0<>, testmodule::dependencies>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                instance<c1>,
                instance<c2>,
                instance<c3>
            >,
            testmodule::pool::sequence
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(externalsbind)
{
    struct testmodule : module
        <
            externals<
                int,
                bind<c1>::in_name<int>,
                bind<c2>::in_call_stack<c1>,
                bind<c3>::in_name<double>::in_call_stack<c4, c5>
            >
        >
    { };

    BOOST_CHECK((equal<vector0<>, testmodule::dependencies>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                instance<int>,
                instance<named<c1, int> >,
                instance<c2, vector<c1> >,
                instance<named<c3, double>, vector<c4, c5> >
            >,
            testmodule::pool::sequence
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(setinstanceInt)
{
    const int i = 42;

    struct testmodule : module
        <
            externals<
                int
            >
        >
    { };

    BOOST_CHECK_EQUAL(i, testmodule::set<int>(i).get());
}

BOOST_AUTO_TEST_CASE(setinstanceannotatein_call_stack)
{
    const int i1 = 42;
    const int i2 = 43;

    struct testmodule : module
        <
            externals<
                annotate< bind<int>::in_call_stack<c1, c2> >::with<A>,
                annotate< bind<int>::in_call_stack<c3, c4> >::with<B>
            >
        >
    { };

    BOOST_CHECK_EQUAL(i1, testmodule::set<A>(i1).get());
    BOOST_CHECK_EQUAL(i2, testmodule::set<A>(i2).get());
}

BOOST_AUTO_TEST_CASE(setinstanceannotatein_name)
{
    const int i1 = 42;
    const int i2 = 43;

    struct testmodule : module
        <
            externals<
                annotate< bind<int>::in_name<float> >::with<A>,
                annotate< bind<int>::in_name<double> >::with<B>
            >
        >
    { };

    BOOST_CHECK_EQUAL(i1, testmodule::set<A>(i1).get());
    BOOST_CHECK_EQUAL(i2, testmodule::set<B>(i2).get());
}

BOOST_AUTO_TEST_CASE(setinstanceannotatein_namein_call_stack)
{
    const int i1 = 42;
    const int i2 = 43;

    struct testmodule : module
        <
            externals<
                annotate< bind<int>::in_call_stack<c1, c2>::in_name<float> >::with<A>,
                annotate< bind<int>::in_name<double>::in_call_stack<c3, c4> >::with<B>
            >
        >
    { };

    BOOST_CHECK_EQUAL(i1, testmodule::set<A>(i1).get());
    BOOST_CHECK_EQUAL(i2, testmodule::set<B>(i2).get());
}

BOOST_AUTO_TEST_CASE(setinstanceMix)
{
    const int i1 = 42;
    const int i2 = 43;
    const int i3 = 44;

    struct testmodule : module
        <
            externals<
                int,
                annotate< bind<int>::in_name<float> >::with<A>,
                annotate< bind<int>::in_call_stack<c1, c2>::in_name<float> >::with<B>
            >
        >
    { };

    BOOST_CHECK_EQUAL(i1, testmodule::set<int>(i1).get());
    BOOST_CHECK_EQUAL(i2, testmodule::set<A>(i2).get());
    BOOST_CHECK_EQUAL(i3, testmodule::set<B>(i3).get());
}

BOOST_AUTO_TEST_CASE(module_ctor_withexternals)
{
    const int i = 42;
    const double d = 87.0;

    typedef module
    <
        externals<
            int,
            double
        >
    >
    testmodule;

    testmodule module(
        testmodule::set<int>(i),
        testmodule::set<double>(d)
    );

    BOOST_CHECK_EQUAL(i, module.get_pool().get< instance<int> >());
    BOOST_CHECK_EQUAL(d, module.get_pool().get< instance<double> >());
}

BOOST_AUTO_TEST_CASE(module_ctor_withexternalsSharedPtr)
{
    const int i = 42;
    shared_ptr<value> v(new value(i));

    typedef module
    <
        External<
            value
        >
    >
    testmodule;

    testmodule module(
        testmodule::set<value>(v)
    );

    BOOST_CHECK_EQUAL(i, get<shared_ptr<value> >(module.get_pool().get< instance<value> >())->i);
}

} // namespace ut
} // namespace base
} // namespace front
} // namespace di

