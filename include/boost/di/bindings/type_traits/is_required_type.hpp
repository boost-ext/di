//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_TYPE_TRAITS_IS_REQUIRED_TYPE_HPP
#define BOOST_DI_BINDINGS_TYPE_TRAITS_IS_REQUIRED_TYPE_HPP

#include "boost/di/aux_/mpl.hpp"
#include "boost/di/type_traits/make_plain.hpp"

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

template<typename TValueType, template<typename...> class TComparator = std::is_same>
struct is_required_type {
    template<typename T>
    using apply = TComparator<
        TValueType
      , typename di::type_traits::make_plain<typename T::type>::type
    >;

    template<typename>
    using eval = std::false_type;
};

template<typename... Ts, template<typename...> class TComparator>
struct is_required_type<type_list<Ts...>, TComparator> {
    template<typename T>
    using apply = or_<
        TComparator<
            typename di::type_traits::make_plain<typename T::type>::type
          , Ts
        >...
    >;

    template<typename>
    using eval = std::false_type;
};

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

#endif

