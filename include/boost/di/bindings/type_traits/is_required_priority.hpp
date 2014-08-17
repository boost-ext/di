//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_TYPE_TRAITS_IS_REQUIRED_PRIORITY_HPP
#define BOOST_DI_BINDINGS_TYPE_TRAITS_IS_REQUIRED_PRIORITY_HPP

#include "boost/di/aux_/mpl.hpp"

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

struct is_required_priority {
    template<typename T>
    using apply = int_<1 + T::dependency::scope::priority>;
};

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

#endif

