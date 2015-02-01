//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_INJECTOR_HPP
#define BOOST_DI_FAKE_INJECTOR_HPP

#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di {

namespace detail {
struct fake_provider {
    template<class, class T, class... TArgs>
    auto get(TArgs&&...) const noexcept {
        return T{};
    }
};
} // namespace detail

template<class TExpected = void>
struct fake_injector {
    using deps = void;

    template<class T>
    auto create() const noexcept {
        return create_impl<T>();
    }

    template<class T>
    auto create_impl() const noexcept {
        return T{};
    }

    struct fake_config {
        detail::fake_provider provider() const noexcept {
            return detail::fake_provider{};
        }
    } config_;

    fake_config& config() {
        return config_;
    }
};

}} // boost::di

#endif

