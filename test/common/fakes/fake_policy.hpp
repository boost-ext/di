//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_POLICY_HPP
#define BOOST_DI_FAKE_POLICY_HPP

#include <boost/mpl/bool.hpp>

namespace boost {
namespace di {

template<int = 0>
class fake_policy
{
public:
    template<typename>
    void assert_policy() const
    { }
};

} // namespace di
} // namespace boost

#endif

