//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_CTOR_CFG_BRACKET

#include "boost/di/injector.hpp"

#include <boost/config.hpp>

#if !defined(BOOST_MSVC)
    #include <tr1/memory>
#else
    #include <memory>
#endif

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

struct sp
{
    BOOST_DI_CTOR(sp, (boost::aux::shared_ptr<int>)) { }
};

BOOST_AUTO_TEST_CASE(lack_of_aux::shared_ptr_fail) {
    injector<>().create<sp>();
}

} // namespace di
} // namespace boost

