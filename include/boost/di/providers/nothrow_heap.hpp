//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDERS_NOTHROW_HEAP_HPP
#define BOOST_DI_PROVIDERS_NOTHROW_HEAP_HPP

#include <new>
#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { namespace providers {

class nothrow_heap {
public:
    template<class TDependency, class TDst, class T = typename TDependency::given, class... TArgs>
    inline auto* get(TArgs&&... args) const noexcept {
        return new (std::nothrow) T{std::forward<TArgs>(args)...};
    }
};

}}} // namespace boost::di::providers

#endif

