//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/boundable.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/bindings.hpp"

namespace boost { namespace di { namespace concepts {

template<class... TArgs>
inline auto make_deps(const TArgs&...) noexcept {
    return aux::type_list<TArgs...>{};
}

test empty_deps = [] {
    auto deps = make_deps();
    static_expect(boundable<decltype(deps)>());
};

test bind_to_type = [] {
    static_expect(boundable<int, int>());
    static_expect(boundable<int, short>());
    static_expect(!boundable<int, std::string>());
};

}}} // boost::di::concepts

