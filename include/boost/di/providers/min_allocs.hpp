//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDERS_MIN_ALLOCS_HPP
#define BOOST_DI_PROVIDERS_MIN_ALLOCS_HPP

#include <new>
#include <utility>
#include "boost/di/aux_/utility.hpp"

namespace boost {
namespace di {
namespace providers {

class min_allocs {
public:
    template<class TDependency, class... TArgs>
    inline typename TDependency::given* get_ptr(TArgs&&... args) const noexcept {
        return new (std::nothrow) typename TDependency::given{std::forward<TArgs>(args)...};
    }

    template<class TDependency, class... TArgs>
    inline typename TDependency::given get_value(TArgs&&... args) const noexcept {
        return typename TDependency::given{std::forward<TArgs>(args)...};
    }

    template<class TDependency, class... TArgs>
    inline aux::enable_if_t<
        std::is_polymorphic<typename TDependency::given>{}
      , typename TDependency::given*
    >
    get(TArgs&&... args) const noexcept {
        return get_ptr<TDependency>(std::forward<TArgs>(args)...);
    }

    template<class TDependency, class... TArgs>
    inline aux::enable_if_t<
        !std::is_polymorphic<typename TDependency::given>{}
      , typename TDependency::given
    >
    get(TArgs&&... args) const noexcept {
        return get_value<TDependency>(std::forward<TArgs>(args)...);
    }
};

} // namespace providers
} // namespace di
} // namespace boost

#endif

