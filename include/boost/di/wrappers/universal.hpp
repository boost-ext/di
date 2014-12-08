//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_UNIVERSAL_HPP
#define BOOST_DI_WRAPPERS_UNIVERSAL_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"

namespace boost { namespace di { namespace wrappers {

template<class T, class TWrapper>
struct universal {
    TWrapper wrapper_;

    inline operator T() const noexcept {
        return wrapper_;
    }
};

template<class TWrapper, class T, class TName>
struct universal<named<T, TName>, TWrapper> {
    TWrapper wrapper_;

    inline operator named<T, TName>() const noexcept {
        return wrapper_.operator T();
    }
};

}}} // boost::di::wrappers

#endif

