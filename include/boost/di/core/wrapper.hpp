//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_WRAPPER_HPP
#define BOOST_DI_CORE_WRAPPER_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/concepts/creatable.hpp"

namespace boost { namespace di { namespace core {

template<class T, class TWrapper, class = void>
struct wrapper {
    using element_type = T;

    inline operator T() const noexcept {
        return wrapper_;
    }

    inline operator T() noexcept {
        return wrapper_;
    }

    TWrapper wrapper_;
};

template<class T, class TWrapper>
struct wrapper<T, TWrapper, BOOST_DI_REQUIRES_T(!std::is_convertible<TWrapper, T>::value)> {
    using element_type = T;

    inline operator T() const noexcept {
        return typename type<TWrapper>::template is_not_convertible_to<T>{};
    }

    inline operator T() noexcept {
        return typename type<TWrapper>::template is_not_convertible_to<T>{};
    }

    TWrapper wrapper_;
};

}}} // boost::di::core

#endif

