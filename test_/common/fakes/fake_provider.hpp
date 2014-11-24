//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_PROVIDER_HPP
#define BOOST_DI_FAKE_PROVIDER_HPP

#include "boost/di/aux_/type_traits.hpp"

#include <utility>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace di {

class fake_provider
{
public:
    template<typename TExpected, typename TGiven>
    typename enable_if<type_traits::has_value<TGiven>, TExpected*>::type provide() const {
        ++provide_calls();
        return new TExpected(TGiven::value);
    }

    template<typename TExpected, typename TGiven, typename... TArgs>
    TExpected* provide(TArgs&&... args) const {
        ++provide_calls();
        return new TGiven(std::forward<TArgs>(args)...);
    }

    static int& provide_calls() {
        static int calls = 0;
        return calls;
    }
};

} // namespace di
} // namespace boost

#endif

