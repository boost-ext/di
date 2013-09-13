//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "boost/di/concepts.hpp"
#include "boost/di/make_injector.hpp"

#include "data.hpp"

#include <boost/units/detail/utility.hpp>
namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(make_injector_ctor) {
    using injector_c0 = injector<
        c0if0
    >;

    auto f = make_injector(
            bind<int>::to(32)
    );

    using injector_c1 = injector<
        per_request<
            double
          , float
        >
    >;

    auto m = injector_c0();
    injector<injector_c0, per_request<int>, injector_c1, decltype(f)> inj(m, f);
    using inj_t = decltype(inj);

    std::cout << "dupa: " << boost::units::detail::demangle(typeid(inj_t::deps::type).name()) << std::endl;

    shared_ptr<c5> c5_ = inj.create<shared_ptr<c5>>();
    //BOOST_CHECK_EQUAL(0, c5_->c2_->i);
    BOOST_CHECK(0);
}

BOOST_AUTO_TEST_CASE(make_injector_by_value) {
    //using injector_c0 = injector<
        //c0if0
    //>;

    //auto injector_c0_int_ = make_injector(
        //injector_c0()
      //, bind<int>::to(42)
    //);

    //shared_ptr<c5> c5_ = injector_c0_int_.create<shared_ptr<c5>>();
    //BOOST_CHECK_EQUAL(42, c5_->c2_->i);
}

} // namespace di
} // namespace boost

