//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_ANNOTATE_HPP
#define BOOST_DI_CONCEPTS_ANNOTATE_HPP

namespace boost {
namespace di {
namespace concepts {

template<typename T>
struct annotate
{
    template<typename TName = void>
    struct with
    {
        typedef with type;
        typedef T element_type;
        typedef TName name;
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

