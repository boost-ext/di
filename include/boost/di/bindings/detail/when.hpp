//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_DETAIL_WHEN_HPP
#define BOOST_DI_BINDINGS_DETAIL_WHEN_HPP

namespace boost {
namespace di {
namespace bindings {
namespace detail {

template<typename T, typename TBind>
using apply_for_all = typename TBind::template apply<T>::type;

template<typename... Ts>
struct when_ {
    template<typename T>
    using apply = typename max<int_<0>, typename apply_for_all<T, Ts>::type...>::type;
};

template<>
struct when_<> {
    template<typename T>
    using apply = int_<1>;
};

} // namespace detail
} // namespace bindings
} // namespace di
} // namespace boost

#endif

