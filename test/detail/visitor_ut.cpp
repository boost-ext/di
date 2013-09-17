//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/visitor.hpp"

#include <boost/test/unit_test.hpp>
#include <typeinfo>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>

#include "fake_binder.hpp"

namespace boost {
namespace di {
namespace detail {

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
struct visitor_mock
{
    template<typename TDependency>
    void operator()() const {
        BOOST_CHECK_EQUAL(
            typeid(T).name()
          , typeid(typename TDependency::given).name()
        );
    }
};

BOOST_AUTO_TEST_CASE(basic) {
    typedef fake_dependency<int> dependency_type;
    typedef fake_binder<dependency_type> binder_type;

    visitor_mock<int> visitor_mock;
    visitor<binder_type>::execute<int, mpl::vector0<> >(visitor_mock);
}

} // namespace detail
} // namespace di
} // namespace boost

