//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_WRAPPER_HPP
#define BOOST_DI_CORE_WRAPPER_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/concepts/creatable.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

namespace successful {

template<class T, class TWrapper>
struct wrapper {
    using value_type = T;

    inline operator T() noexcept {
        return BOOST_DI_TYPE_WKND(T)wrapper_;
    }

    TWrapper wrapper_;
};

} // successful

template<class T, class TWrapper, class = int>
struct wrapper_impl {
    using value_type = T;

    inline operator T() noexcept {
        return wrapper_;
    }

    TWrapper wrapper_;
};

template<class T, class TWrapper>
struct wrapper_impl<T, TWrapper, BOOST_DI_REQUIRES(!aux::is_convertible<TWrapper, T>::value)> {
    using value_type = T;

    inline operator T() noexcept {
        return typename concepts::type<TWrapper>::template is_not_convertible_to<T>{};
    }

    TWrapper wrapper_;
};

template<class T, class TWrapper>
using wrapper = wrapper_impl<T, TWrapper>;

}}}} // boost::di::v1::core

#endif

