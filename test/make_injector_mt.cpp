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
#include "boost/di/make_module.hpp"

#include "data.hpp"

namespace boost {
namespace di {

using module_c0 = module<
    c0if0
>;

auto module_c0_int_ = make_module(
    module_c0()
  , bind<int>::to(42)
);

BOOST_AUTO_TEST_CASE(injector_ctor) {
    injector<module_c0> inj;
    shared_ptr<c5> c5_ = inj.create<shared_ptr<c5>>();
    BOOST_CHECK_EQUAL(0, c5_->c2_->i);
}

BOOST_AUTO_TEST_CASE(injector_install_by_value) {
    injector<> inj;
    shared_ptr<c5> c5_ = inj.install(module_c0()).create<shared_ptr<c5>>();
    BOOST_CHECK_EQUAL(0, c5_->c2_->i);
}

BOOST_AUTO_TEST_CASE(injector_install_by_ref) {
    injector<> inj;
    shared_ptr<c5> c5_ = inj.install(module_c0_int_).create<shared_ptr<c5>>();
    BOOST_CHECK_EQUAL(42, c5_->c2_->i);
}

BOOST_AUTO_TEST_CASE(make_injector_module) {
    auto inj = make_injector(module_c0_int_);
    shared_ptr<c5> c5_ = inj.create<shared_ptr<c5>>();
    BOOST_CHECK_EQUAL(42, c5_->c2_->i);
}

} // namespace di
} // namespace boost

