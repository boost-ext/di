//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/check_for_undefined_behaviors.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>

#include "data.hpp"
#include "fake_dependency.hpp"
#include "fake_dependency_base_of.hpp"

#include <boost/di.hpp>

#include <boost/units/detail/utility.hpp>

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(basic) {

    detail::module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, int, mpl::int_<42>, mpl::vector<ub2, ub1>>::type
          , fake_dependency_base_of<scopes::singleton<>, ub1>::type
        >
    > m;

    auto mm = m.create<ub3>();

    std::cout << mm.ub1_->i_ << std::endl;
    std::cout << mm.ub2_->ub1_->i_ << std::endl;
    BOOST_CHECK_EQUAL(mm.ub1_->i_, mm.ub2_->ub1_->i_);

    using m_t = check_for_undefined_behaviors::verify<
        mpl::vector<
            fake_dependency<scopes::per_request<>, int, mpl::int_<42>, mpl::vector<ub2, ub1>>::type
          , fake_dependency_base_of<scopes::singleton<>, ub1>::type
        >
      , ub3
    >;

    using mm_t = decltype(mm);

    std::cout << units::detail::demangle(typeid(m_t::s::type).name()) << std::endl;
    BOOST_CHECK(false);
}

} // namespace policies
} // namespace di
} // namespace boost

