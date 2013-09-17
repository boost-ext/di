//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_BINDER_HPP
#define BOOST_DI_FAKE_BINDER_HPP

namespace boost {
namespace di {

template<typename T>
struct fake_binder
{
    template<
        typename
      , typename
    >
    struct get_dependency
    {
        typedef T type;
    };
};

} // namespace di
} // namespace boost

#endif

