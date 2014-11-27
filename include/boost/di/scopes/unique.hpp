//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_UNIQUE_HPP
#define BOOST_DI_SCOPES_UNIQUE_HPP

#include "boost/di/wrappers/copy.hpp"

namespace boost {
namespace di {
namespace scopes {

class unique {
public:
    static constexpr auto priority = 0; // 0 - lowest, N - highest

    template<class>
    struct rebind {
        using other = unique;
    };

    template<class T>
    class scope {
    public:
        void create3(int);

        template<typename TProvider>
        wrappers::copy<T*> create(const TProvider& provider) { return provider.get(); }

        //wrappers:provider.get():copy<T> create(T&& ptr) { return std::move(provider.get()); }
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

