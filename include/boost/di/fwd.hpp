//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FWD_HPP
#define BOOST_DI_FWD_HPP

#include "boost/di/fwd_ext.hpp"

struct no_name {
    constexpr auto operator()() const noexcept { return ""; }
};

template<class, class = void> struct named { };
template<class, class = int> struct ctor_traits;

namespace core {
    template<class> struct any_type_fwd;
    template<class> struct any_type_ref_fwd;
    template<class> struct any_type_1st_fwd;
    template<class> struct any_type_1st_ref_fwd;

    template<class T>
    struct dependency__ : T {
        using T::try_create;
        using T::is_referable;
        using T::create;
    };

    template<class T>
    struct injector__ : T {
        using T::try_create;
        using T::create_impl;
        using T::create_successful_impl;

        #if defined(_MSC_VER) // __pph__
            template<class... Ts> using is_creatable =
                typename T::template is_creatable<Ts...>;
        #else // __pph__
            using T::is_creatable;
        #endif // __pph__
    };

    template<class, class...>
    struct array;

} // core

//TODO remove
namespace concepts { template<class...> struct boundable__; template<class...> struct any_of; } // concepts

#endif

