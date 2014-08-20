//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_PROVIDER_HPP
#define BOOST_DI_FAKE_PROVIDER_HPP

#include <utility>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace di {

class fake_provider {
    BOOST_DI_HAS_MEMBER(value);

public:
    template<typename TExpected, typename TGiven>
    typename enable_if<has_value<TGiven>, TExpected*>::type get() const {
        ++get_calls();
        return new TExpected(TGiven::value);
    }

    template<typename TExpected, typename TGiven, typename... TArgs>
    TExpected* get(TArgs&&... args) const {
        ++get_calls();
        return new TGiven(std::forward<TArgs>(args)...);
    }

    static int& get_calls() {
        static int calls = 0;
        return calls;
    }
};

} // namespace di
} // namespace boost

#endif

