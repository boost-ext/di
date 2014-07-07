//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_ALLOCATOR_HPP
#define BOOST_DI_FAKE_ALLOCATOR_HPP

#include "boost/di/type_traits/has_value.hpp"

#include <utility>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace di {

class fake_allocator
{
public:

    template<typename TExpected, typename TGiven>
    typename enable_if<type_traits::has_value<TGiven>, TExpected*>::type allocate() const {
        return new TExpected(TGiven::value);
    }

    template<typename TExpected, typename TGiven, typename... TArgs>
    TExpected* allocate(TArgs&&... args) const {
        return new TGiven(std::forward<TArgs>(args)...);
    }
};

} // namespace di
} // namespace boost

#endif

