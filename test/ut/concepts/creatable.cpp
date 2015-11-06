//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/creatable.hpp"

namespace concepts {

struct d {
    d(int, double) { }
};

struct u {
    int i = {};
    double d = {};
};

test is_creatable = [] {
    expect(creatable<type_traits::direct, int>::value);
    expect(creatable<type_traits::uniform, int>::value);
    expect(creatable<type_traits::direct, d, int, double>::value);
    expect(!creatable<type_traits::direct, d, int>::value);
    expect(creatable<type_traits::uniform, d, int, double>::value);
    expect(!creatable<type_traits::uniform, d, int>::value);
    expect(!creatable<type_traits::direct, u, int, double>::value);
    expect(creatable<type_traits::uniform, u>::value);
    expect(!creatable<type_traits::uniform, u, float, short>::value);

#if !defined(_MSC_VER)
    expect(creatable<type_traits::uniform, u, int, double>::value);
    expect(creatable<type_traits::uniform, u, int>::value);
#endif
};

} // concepts

