// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_DEDUCE_HPP
#define BOOST_DI_SCOPES_DEDUCE_HPP

#include "boost/di/type_traits/scope_traits.hpp"

namespace boost { namespace di { namespace scopes {

class deduce {
public:
    static constexpr auto priority = false;

    template<class TExpected, class T>
    class scope {
    public:
        template<class TDst, class TProvider>
        decltype(auto) create(const TProvider& provider) const noexcept {
            using scope_traits = type_traits::scope_traits_t<TDst>;
            using scope = typename scope_traits::template scope<TExpected, T>;
            return scope{}.template create<TDst>(provider);
        }
    };
};

}}} // boost::di::scopes

#endif

