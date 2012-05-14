//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_ANNOTATE_HPP
#define BOOST_DI_CONCEPTS_ANNOTATE_HPP

#include <boost/none_t.hpp>
#include <boost/mpl/vector.hpp>
#include "boost/di/aux_/instance.hpp"

namespace boost {
namespace di {
namespace concepts {

template<typename TDerived = none_t>
struct annotate
{
    template<typename TName = void>
    struct with : TDerived
    {
        typedef TDerived derived;
        typedef TName name;
    };
};

template<>
struct annotate<none_t>
{
    template<typename TName = void>
    struct with
    {
        typedef none_t derived;
        typedef TName name;
    };

    template<
        typename TExpected
      , typename TContext = mpl::vector0<>
      , template<
            typename = TExpected
          , typename = TContext
          , typename = void
        > class TInstance = aux_::instance
    >
    struct with_ : with<>
    {
        template<typename T>
        static TInstance<> to(T value) {
            return TInstance<>(value);
        }
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

