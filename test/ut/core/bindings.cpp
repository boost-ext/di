//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/bindings.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "common/fakes/fake_dependency.hpp"

namespace core {

template<class... TDeps>
struct fake_injector {
    using deps = aux::type_list<TDeps...>;
};

test bindings_deps = [] {
    static_expect(std::is_same<aux::type_list<>, bindings_t<>>::value);
    static_expect(std::is_same<aux::type_list<fake_dependency<int>>, bindings_t<fake_dependency<int>>>::value);
    static_expect(std::is_same<aux::type_list<fake_dependency<int>, fake_dependency<double>>, bindings_t<fake_dependency<int>, fake_dependency<double>>>::value);
    static_expect(std::is_same<aux::type_list<fake_dependency<int>, dependency<scopes::exposed<>, int>>, bindings_t<fake_dependency<int>, int>>::value);
    static_expect(std::is_same<aux::type_list<fake_dependency<int>, fake_dependency<double>, dependency<scopes::exposed<>, int>>, bindings_t<fake_injector<fake_dependency<int>, fake_dependency<double>>, int>>::value);
};

} // core

