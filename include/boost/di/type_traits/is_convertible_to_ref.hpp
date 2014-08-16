
//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_IS_CONVERTIBLE_TO_REF_HPP
#define BOOST_DI_TYPE_TRAITS_IS_CONVERTIBLE_TO_REF_HPP

#include "boost/di/aux_/mpl.hpp"

namespace boost {
namespace di {
namespace type_traits {

namespace detail {

template<typename T, typename TSignature>
class is_convertible {
    template<typename U>
    static yes_tag test(non_type<TSignature, &U::operator()>*);

    template<typename>
    static no_tag test(...);

public:
    static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_tag);
};

} // namespace detail

template<typename TValueType, typename T>
using is_convertible_to_ref = detail::is_convertible<
    TValueType, const T&(TValueType::*)(const type<const T&>&) const
>;

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

