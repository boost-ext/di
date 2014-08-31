//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_DETAIL_REQUIRES_HPP
#define BOOST_DI_BINDINGS_DETAIL_REQUIRES_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/mpl.hpp"

namespace boost {
namespace di {
namespace bindings {
namespace detail {

template<typename T, typename TBind>
struct apply_bind : TBind::template apply<T>::type { };

template<typename T, typename TBind>
struct eval_bind : TBind::template eval<T>::type { };

template<typename... Ts>
struct requires_ {
    template<typename T>
    using apply = and_<typename apply_bind<T, Ts>::type...>;

    template<typename T>
    using eval = and_<typename eval_bind<T, Ts>::type...>;
};

} // namespace detail
} // namespace bindings
} // namespace di
} // namespace boost

#endif

