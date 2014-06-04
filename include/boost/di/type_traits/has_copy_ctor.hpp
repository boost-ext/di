//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_HAS_COPY_CTOR_HPP
#define BOOST_DI_TYPE_TRAITS_HAS_COPY_CTOR_HPP

#include "boost/di/aux_/config.hpp"

#include <boost/mpl/aux_/yes_no.hpp>

namespace boost {
namespace di {
namespace type_traits {

template<typename T>
class has_copy_ctor
{
    static T copy_ctor();

    template<typename U>
    static mpl::aux::yes_tag test(BOOST_DI_FEATURE_DECLTYPE(U(copy_ctor()))*);

    template<typename>
    static mpl::aux::no_tag test(...);

public:
    typedef has_copy_ctor type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
    );
};

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

