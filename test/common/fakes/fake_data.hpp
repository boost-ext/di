//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DATA_HPP
#define BOOST_DI_FAKE_DATA_HPP

namespace boost {
namespace di {

template<
    typename T
  , typename TCallStack
  , typename TScope
>
struct fake_data {
    using type = T;
    using call_stack = TCallStack;
    struct dependency {
        using scope = TScope;
    };
};

} // namespace di
} // namespace boost

#endif

