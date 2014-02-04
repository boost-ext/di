//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/injector.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

struct ctor1
{
	ctor1(int) { } // error
	//BOOST_DI_INJECT(ctor1, (int)) { } // ok
	//ctor1(int, int = 0) { } // ok
};

BOOST_AUTO_TEST_CASE(ctor_one_arg_fail) {
    injector<>().create<ctor1>();
}

} // namespace di
} // namespace boost

