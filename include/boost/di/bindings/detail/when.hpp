//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_DETAIL_WHEN_HPP
#define BOOST_DI_BINDINGS_DETAIL_WHEN_HPP

#include "boost/di/aux_/mpl.hpp"

namespace boost {
namespace di {
namespace bindings {
namespace detail {

template<typename T, typename TBind>
struct apply_for_all :  TBind::template apply<T>::type { };

template<typename T, typename TBind>
struct eval_for_all : TBind::template eval<T>::type { };

template<typename... Ts>
struct when_ {
    using type = when_;

    template<typename T>
    using apply = typename max<int_<0>, typename apply_for_all<T, Ts>::type...>::type;

    template<typename T>
    using eval = typename max<int_<0>, typename eval_for_all<T, Ts>::type...>::type;
};

template<>
struct when_<> {
    using type = when_;

    template<typename T>
    using apply = int_<1>;

    template<typename>
    using eval = int_<0>;
};

} // namespace detail
} // namespace bindings
} // namespace di
} // namespace boost

#endif

