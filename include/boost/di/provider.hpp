//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDER_HPP
#define BOOST_DI_PROVIDER_HPP

#include <utility>

namespace boost {
namespace di {

class provider {
public:
    template<typename TExpected, typename TGiven, typename... TArgs>
    TExpected* get(TArgs&&... args) const {
        return new TGiven(std::forward<TArgs>(args)...);
    }
};

} // namespace di
} // namespace boost

#endif
