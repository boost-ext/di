//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDER_HPP
#define BOOST_DI_PROVIDER_HPP

#include <utility>
#include <new>

namespace boost {
namespace di {

class provider {
public:
    template<typename TDependency, typename... TArgs>
    typename TDependency::expected* get(TArgs&&... args) const noexcept {
        return new (std::nothrow) typename TDependency::given(std::forward<TArgs>(args)...);
    }
};

} // namespace di
} // namespace boost

#endif

