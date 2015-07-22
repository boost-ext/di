//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

namespace di = boost::di;

struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
struct impl : i { void dummy() override { } };

struct c {
    c(i*){}
};

test create_polymorphic_type_without_binding = [] {
    di::make_injector().create<c>();
};

