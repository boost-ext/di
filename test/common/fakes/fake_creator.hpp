//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_CREATOR_HPP
#define BOOST_DI_FAKE_CREATOR_HPP

#include <typeinfo>
#include <boost/test/unit_test.hpp>

#include "boost/di/type_traits/remove_accessors.hpp"

namespace boost {
namespace di {

template<typename TExpected = void>
struct fake_creator
{
    template<
        typename T
      , typename TParent
      , typename TCallStack
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TArgs
    >
    T create(TDeps&, TRefs&, const TVisitor&, const TArgs&) {
        BOOST_CHECK_EQUAL(typeid(TExpected).name(), typeid(T).name());
        static typename type_traits::remove_accessors<T>::type object;
        return object;
    }
};

} // namespace di
} // namespace boost

#endif

