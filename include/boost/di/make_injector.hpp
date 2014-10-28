//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_MAKE_INJECTOR_HPP
#define BOOST_DI_MAKE_INJECTOR_HPP

#include "boost/di/aux_/mpl.hpp"
#include "boost/di/injector.hpp"

namespace boost {
namespace di {

template<typename... TArgs>
inline auto make_injector(const TArgs&... args) {
    return injector<typename bindings<TArgs...>::type>(args...);
}

} // namespace di
} // namespace boost

#endif

