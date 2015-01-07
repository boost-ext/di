//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/injector.hpp"
#include "common/fakes/fake_dependency.hpp"

namespace boost { namespace di {

test injector_empty = [] {
    using injector = injector<>;
    expect(std::is_same<aux::type_list<>, injector::deps>{});
};

test injector_exposed = [] {
    using injector = injector<int>;
    using dep = core::dependency<scopes::exposed<scopes::deduce>, int>;
    expect(std::is_same<aux::type_list<dep>, injector::deps>{});
};

test injector_with_injector = [] {
};

test injector_with_module = [] {
};

}} // boost::di

