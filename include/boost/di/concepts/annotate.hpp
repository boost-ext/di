//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_ANNOTATE_HPP
#define BOOST_DI_CONCEPTS_ANNOTATE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>

#include "boost/di/type_traits/is_same_base_of.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/detail/dependency.hpp"

namespace boost {
namespace di {
namespace concepts {

template<typename T = void>
struct annotate
{
    template<typename TName = void>
    struct with : T
    {
        typedef T element_type;
        typedef TName name;
    };
};

template<>
struct annotate<void>
{
    template<typename TName = void>
    struct with
    {
        typedef TName name;
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

