//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/creatable.hpp"

namespace boost { namespace di { namespace concepts {

struct d {
    d(int, double) { }
};

struct u {
    int i;
    double d;
};

test is_creatable = [] {
    expect(creatable<int>(type_traits::direct{}));
    expect(creatable<int>(type_traits::uniform{}));

    expect(creatable<d, int, double>(type_traits::direct{}));
    expect(!creatable<d, int>(type_traits::direct{}));
    expect(creatable<d, int, double>(type_traits::uniform{}));
    expect(!creatable<d, int>(type_traits::uniform{}));

    expect(!creatable<u, int, double>(type_traits::direct{}));
    expect(creatable<u, int, double>(type_traits::uniform{}));
    expect(creatable<u, int>(type_traits::uniform{}));
    expect(creatable<u>(type_traits::uniform{}));
    expect(!creatable<u, float, short>(type_traits::uniform{}));
};

}}} // boost::di::concepts

