//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DEPENDENCY_HPP
#define BOOST_DI_FAKE_DEPENDENCY_HPP

#include "boost/di/fwd.hpp"
#include "common/fakes/fake_scope.hpp"

namespace boost { namespace di {

template<class T, class TScope = fake_scope<>>
struct fake_dependency : TScope::template scope<T, T> {
    using expected = T;
    using given = T;
    using scope = TScope;
    using name = no_name;
};

}} // boost::di

#endif

