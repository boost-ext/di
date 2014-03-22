//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_POOL_HPP
#define BOOST_DI_FAKE_POOL_HPP

#include <boost/mpl/vector.hpp>

namespace boost {
namespace di {

template<typename... T>
struct fake_pool : T...
{
    typedef mpl::vector<T...> types;
};

} // namespace di
} // namespace boost

#endif

