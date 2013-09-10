//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/concepts.hpp"
#include "boost/di/module.hpp"
#include "boost/di/make_injector.hpp"
#include "boost/di/make_module.hpp"

#include "data.hpp"

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(injector_) {
typedef module<

   //per_request<
        bind_int<0>
      , bind_int<1>
    //>
> module_2;
    typedef module<
        c0if0
    > mm;

    //auto mm = module2<>()(
        //singleton<
            //c0if0
        //>()
    //);

    //auto mod = make_module(m());

    injector<> inj;
    inj.install(module_2());

    //auto inj = make_injector(mod);
    //make_injector(m());

    //shared_ptr<c5> c5_ = inj.create<shared_ptr<c5>>();
    //BOOST_CHECK_EQUAL(42, c5_->c2_->i);
}

} // namespace di
} // namespace boost

