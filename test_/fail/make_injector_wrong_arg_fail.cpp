//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/make_injector.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

struct neither_module_nor_injector_nor_concept { };

BOOST_AUTO_TEST_CASE(wrong_arg_fail) {
    make_injector(neither_module_nor_injector_nor_concept());
}

} // namespace di
} // namespace boost


