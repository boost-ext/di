//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_CONCEPT_HPP
#define BOOST_DI_FAKE_CONCEPT_HPP

#include <boost/mpl/int.hpp>

namespace boost {
namespace di {

template<int Value>
struct fake_concept
{
    template<typename>
    struct apply
        : mpl::int_<Value>
    { };
};

} // namespace di
} // namespace boost

#endif


