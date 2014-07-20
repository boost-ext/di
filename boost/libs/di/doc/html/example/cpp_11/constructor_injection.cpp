//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[constructor_injection_cpp_11
//````C++11```
//<-
#include <cassert>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct name { };

struct ctor_named {
    /*<<constructor with intrusive named parameter>>*/
    ctor_named(int i1, di::named<int, name> i2 = 0) {
        assert(i1 == 0);
        assert(i2 == 42);
    }
};

struct ctor_inject {
    /*<<constructor with intrusive named parameter explicitly selected>>*/
    BOOST_DI_INJECT(ctor_inject, int i1, di::named<int, name> i2 = 0) {
        assert(i1 == 0);
        assert(i2 == 42);
    }
};

struct ctor_inject_traits {
    /*<<constructor with less intrusive named parameter using traits>>*/
    BOOST_DI_INJECT_TRAITS(int, di::named<int, name>);
    ctor_inject_traits(int i1, int i2 = 0) {
        assert(i1 == 0);
        assert(i2 == 42);
    }
};

struct ctor_injector {
    /*<<constructor with less intrusive named parameter using static method>>*/
    static void BOOST_DI_INJECTOR(int, di::named<int, name>);
    ctor_injector(int i1, int i2 = 0) {
        assert(i1 == 0);
        assert(i2 == 42);
    }
};

struct ctor_di_traits {
    /*<<class without any changes>>*/
    ctor_di_traits(int i1, int i2 = 0) {
        assert(i1 == 0);
        assert(i2 == 42);
    }
};

namespace boost {
namespace di {

template<>
struct ctor_traits<ctor_di_traits> {
    /*<<no intrusive way of defining named parameters>>*/
    BOOST_DI_INJECT_TRAITS(int, di::named<int, name>);
};

} // namespace di
} // namespace boost

int main() {
    /*<<make injector>>*/
    auto injector = di::make_injector(
        di::bind_int<42>::named<name>()
    );

    /*<<create dependencies>>*/
    injector.create<ctor_named>();
    injector.create<ctor_inject>();
    injector.create<ctor_inject_traits>();
    injector.create<ctor_injector>();
    injector.create<ctor_di_traits>();
}

//]

