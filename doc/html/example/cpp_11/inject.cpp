//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[inject_cpp_11
//`[h6 C++ 11]
//<-
#include <cassert>
//->
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct name { };

struct c0
{
    c0(int i1, di::named<int, name> i2 = 0) {
        assert(i1 == 0);
        assert(i2 == 42);
    }
};

struct c1
{
    BOOST_DI_INJECT(c1, int i1, di::named<int, name> i2 = 0) {
        assert(i1 == 0);
        assert(i2 == 42);
    }
};

struct c2
{
    BOOST_DI_INJECT_TRAITS(int, di::named<int, name>);
    c2(int i1, int i2 = 0) {
        assert(i1 == 0);
        assert(i2 == 42);
    }
};

struct c3
{
    static void BOOST_DI_INJECTOR(int, di::named<int, name>);
    c3(int i1, int i2 = 0) {
        assert(i1 == 0);
        assert(i2 == 42);
    }
};

struct c4
{
    c4(int i1, int i2 = 0) {
        assert(i1 == 0);
        assert(i2 == 42);
    }
};

} // namespace

namespace boost {
namespace di {

template<>
struct ctor_traits<c4>
{
    BOOST_DI_INJECT_TRAITS(int, di::named<int, name>);
};

} // namespace di
} // namespace boost

int main() {
    di::injector<
        di::bind_int<42>::named<name>
    > injector;

    injector.create<c0>();
    injector.create<c1>();
    injector.create<c2>();
    injector.create<c3>();
    injector.create<c4>();
}

//`full code example: [@example/cpp_11/inject.cpp inject.cpp]
//]

