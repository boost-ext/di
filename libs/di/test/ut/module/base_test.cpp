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

using namespace test::common;
using namespace back;
using namespace back::aux;
using namespace back::scopes;
using namespace utility;
using namespace boost::mpl;
using namespace boost;

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

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(defaultscope)
{
    struct testmodule : module<C1> { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::per_request, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(defaultscopeMany)
{
    struct testmodule : module<C1, C2, C3> { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::per_request, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                dependency<back::scopes::per_request, C2, C2, vector0<>, or_< is_base_of<_1, C2>, is_same<_1, C2> > >,
                dependency<back::scopes::per_request, C3, C3, vector0<>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(defaultscopebind)
{
    struct testmodule : module
        <
            bind<If0, CIf0>,
            C1,
            bind<C2>::in_name<int>,
            bind<C3>::in_call_stack<C4, C5>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::per_request, If0, CIf0, vector0<>, is_same<_1, If0> >,
                dependency<back::scopes::per_request, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                dependency<back::scopes::per_request, named<C2, int>, C2, vector0<>, or_< is_base_of<_1, named<C2, int> >, is_same<_1, named<C2, int> > > >,
                dependency<back::scopes::per_request, C3, C3, vector<C4, C5>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(Onescope)
{
    struct testmodule : module
        <
            scope<back::scopes::singleton>::bind <
                CIf0
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(OnescopeAlias)
{
    struct testmodule : module
        <
            singletons <
                CIf0
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(OnescopeDirect)
{
    struct testmodule : module
        <
            singleton<CIf0>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(Many)
{
    struct testmodule : module
        <
            singletons<
                C1,
                C2,
                C3
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                dependency<back::scopes::singleton, C2, C2, vector0<>, or_< is_base_of<_1, C2>, is_same<_1, C2> > >,
                dependency<back::scopes::singleton, C3, C3, vector0<>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(Manyscopes)
{
    struct testmodule : module
        <
            singletons<
                C1,
                C2
            >,
            per_requests<
                C3,
                C4
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                dependency<back::scopes::singleton, C2, C2, vector0<>, or_< is_base_of<_1, C2>, is_same<_1, C2> > >,
                dependency<back::scopes::per_request, C3, C3, vector0<>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >,
                dependency<back::scopes::per_request, C4, C4, vector0<>, or_< is_base_of<_1, C4>, is_same<_1, C4> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(in_call_stack)
{
    struct testmodule : module
        <
            per_request<C1>::in_call_stack<C2>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::per_request, C1, C1, vector<C2>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(in_name)
{
    struct testmodule : module
        <
            singleton<C1>::in_name<int>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, named<C1, int>, C1, vector0<>, or_< is_base_of<_1, named<C1, int> >, is_same<_1, named<C1, int> > > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(in_namein_call_stack)
{
    struct testmodule : module
        <
            singletons<
                bind<C1>::in_name<int>::in_call_stack<double>,
                bind<C2>::in_name<double>::in_call_stack<int>
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, named<C1, int>, C1, vector<double>, or_< is_base_of<_1, named<C1, int> >, is_same<_1, named<C1, int> > > >,
                dependency<back::scopes::singleton, named<C2, double>, C2, vector<int>, or_< is_base_of<_1,  named<C2, double> >, is_same<_1,  named<C2, double> > > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(in_call_stackin_name)
{
    struct testmodule : module
        <
            singletons<
                bind<C1>::in_call_stack<double>::in_name<int>,
                bind<C2>::in_call_stack<int>::in_name<double>
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, named<C1, int>, C1, vector<double>, or_< is_base_of<_1, named<C1, int> >, is_same<_1, named<C1, int> > > >,
                dependency<back::scopes::singleton, named<C2, double>, C2, vector<int>, or_< is_base_of<_1,  named<C2, double> >, is_same<_1,  named<C2, double> > > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(bindIf)
{
    struct testmodule : module
        <
            singletons<
                bind<If0, CIf0>
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, If0, CIf0, vector0<>, is_same<_1, If0> >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(Mix)
{
    struct testmodule : module
        <
            singletons<
                bind<If0, CIf0>,
                C1,
                bind<C2>::in_name<int>,
                bind<C3>::in_call_stack<C4, C5>
            >,
            per_requests <
                C6
            >,
            singleton<C7>::in_name<double>::in_call_stack<C1>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, If0, CIf0, vector0<>, is_same<_1, If0> >,
                dependency<back::scopes::singleton, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                dependency<back::scopes::singleton, named<C2, int>, C2, vector0<>, or_< is_base_of<_1, named<C2, int> >, is_same<_1, named<C2, int> > > >,
                dependency<back::scopes::singleton, C3, C3, vector<C4, C5>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >,
                dependency<back::scopes::per_request, C6, C6, vector0<>, or_< is_base_of<_1, C6>, is_same<_1, C6> > >,
                dependency<back::scopes::singleton, named<C7, double>, C7, vector<C1>, or_< is_base_of<_1, named<C7, double> >, is_same<_1, named<C7, double> > > >
            >,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(namedin_call_stack)
{
    struct testmodule : front::base::module
        <
            per_requests <
                bind<int, int_<1> >,
                bind<int, int_<4> >::in_name< string<'2'> >::in_call_stack<C7, C6, C4>,
                bind<int, int_<5> >::in_call_stack<C2>
            >
        >
    { };

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::per_request, int, int_<1>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
                dependency<back::scopes::per_request, named<int, string<'2'> >, int_<4>, vector<C7, C6, C4>, or_< is_base_of<_1, named<int, string<'2'> > >, is_same<_1, named<int, string<'2'> > > > >,
                dependency<back::scopes::per_request, int, int_<5>, vector<C2>, or_< is_base_of<_1, int>, is_same<_1, int> > >
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
                C1
            >
        >
    { };

    BOOST_CHECK((equal<vector0<>, testmodule::dependencies>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                instance<C1>
            >,
            testmodule::pool::seq
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(externalsMix)
{
    struct testmodule : module
        <
            externals<
                C1,
                C2
            >,
            External<C3>
        >
    { };

    BOOST_CHECK((equal<vector0<>, testmodule::dependencies>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                instance<C1>,
                instance<C2>,
                instance<C3>
            >,
            testmodule::pool::seq
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(externalsbind)
{
    struct testmodule : module
        <
            externals<
                int,
                bind<C1>::in_name<int>,
                bind<C2>::in_call_stack<C1>,
                bind<C3>::in_name<double>::in_call_stack<C4, C5>
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
                instance<named<C1, int> >,
                instance<C2, vector<C1> >,
                instance<named<C3, double>, vector<C4, C5> >
            >,
            testmodule::pool::seq
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
                annotate< bind<int>::in_call_stack<C1, C2> >::with<A>,
                annotate< bind<int>::in_call_stack<C3, C4> >::with<B>
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
                annotate< bind<int>::in_call_stack<C1, C2>::in_name<float> >::with<A>,
                annotate< bind<int>::in_name<double>::in_call_stack<C3, C4> >::with<B>
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
                annotate< bind<int>::in_call_stack<C1, C2>::in_name<float> >::with<B>
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

    BOOST_CHECK_EQUAL(i, boost::get<shared_ptr<value> >(module.get_pool().get< instance<value> >())->i);
}

} // namespace ut
} // namespace base
} // namespace front
} // namespace di

