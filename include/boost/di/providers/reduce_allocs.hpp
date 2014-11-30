//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDERS_MIN_ALLOCS_HPP
#define BOOST_DI_PROVIDERS_MIN_ALLOCS_HPP

#include <new>
#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { namespace providers {

class reduce_allocs {
public:
    template<class T, class... TArgs>
    inline T* get_ptr(TArgs&&... args) const noexcept {
        return new (std::nothrow) T{std::forward<TArgs>(args)...};
    }

    template<class T, class... TArgs>
    inline T get_value(TArgs&&... args) const noexcept {
        return T{std::forward<TArgs>(args)...};
    }

    template<class T, class... TArgs>
    inline std::enable_if_t<std::is_polymorphic<T>{}, T*>
    get(TArgs&&... args) const noexcept {
        return get_ptr<T>(std::forward<TArgs>(args)...);
    }

    template<class T, class... TArgs>
    inline std::enable_if_t<!std::is_polymorphic<T>{}, T>
    get(TArgs&&... args) const noexcept {
        return get_value<T>(std::forward<TArgs>(args)...);
    }
};

}}} // namespace boost::di::providers

#endif

