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
    template<typename T>
    typename enable_if<
        type_traits::has_value<typename T::dependency::given>
      , typename T::dependency::expected*
    >::type allocate() const {
        ++allocate_calls();
        return new typename T::dependency::expected(T::dependency::given::value);
    }

    template<typename T, typename... TArgs>
    typename T::dependency::expected* allocate(TArgs&&... args) const {
        ++allocate_calls();
        return new typename T::dependency::given(std::forward<TArgs>(args)...);
    }

    static int& allocate_calls() {
        static int calls = 0;
        return calls;
    }
};

} // namespace di
} // namespace boost

#endif

