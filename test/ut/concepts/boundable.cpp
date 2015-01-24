//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/boundable.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/bindings.hpp"

namespace boost { namespace di { namespace concepts {

template<class TDeps>
decltype(boundable(std::declval<TDeps>())) boundable_deps_v{};

template<class I, class T>
decltype(boundable(std::declval<I>(), std::declval<T>())) boundable_v{};

template<class... TArgs>
inline auto make_deps(const TArgs&...) noexcept {
    return aux::type_list<TArgs...>{};
}

test empty_deps = [] {
    auto deps = make_deps();
    static_assert(boundable_deps_v<decltype(deps)>, "");
};

test bind_to_type = [] {
    static_assert(boundable_v<int, int>, "");
    static_assert(boundable_v<int, short>, "");
    static_assert(!boundable_v<int, std::string>, "");
};

}}} // boost::di::concepts

