//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct name { };

struct c0 { };

struct c1
{
    BOOST_DI_CTOR(c1, int, di::named<int, name> = 0) { }
};

struct c2
{
    BOOST_DI_CTOR_TRAITS(int, di::named<int, name>);
    c2(int, int = 0) { }
};

struct c3
{
    c3(int, int) { }
};

} // namespace

namespace boost {
namespace di {

template<>
struct ctor_traits<c3>
{
    static void ctor(int, di::named<int, name>);
};

} // namespace di
} // namespace boost

int main()
{
    di::injector<> injector;
    injector.create<c0>();
    injector.create<c1>();
    injector.create<c2>();
    injector.create<c3>();
}

