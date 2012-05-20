//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/aux_/visitor.hpp"

#include <boost/test/unit_test.hpp>
#include <typeinfo>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost {
namespace di {
namespace aux_ {

template<typename T>
struct fake_dependency
{
    typedef T given;
    typedef T expected;
    typedef mpl::vector0<> context;
    typedef void scope;
    typedef is_same<mpl::_1, T> bind;
    typedef mpl::vector0<> ctor;
};

template<typename T>
struct fake_binder
{
    template<
        typename
      , typename
    >
    struct impl
    {
        typedef T type;
    };
};

template<typename T>
struct visitor_mock
{
    template<typename TDependency>
    void operator()() const {
        BOOST_CHECK_EQUAL(typeid(T).name(), typeid(typename TDependency::given).name());
    }
};

BOOST_AUTO_TEST_CASE(basic) {
    typedef fake_dependency<int> dependency_t;
    visitor_mock<int> visitor_mock;

    visitor<fake_binder<dependency_t> >::execute<int, mpl::vector0<> >(visitor_mock);
}

} // namespace aux_
} // namespace di
} // namespace boost

