//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct c0
{ };

struct c1
{
    BOOST_DI_CTOR(c1, boost::shared_ptr<c0>, int, double) { }
};

struct hello_world
{
    BOOST_DI_CTOR(hello_world, boost::shared_ptr<c1>) { }
};

} // namespace

int main()
{
    di::injector<> injector;

    {
        injector.create<hello_world>();
    }

    {
        injector.create<boost::shared_ptr<hello_world>>();
    }

    return 0;
}

#include <boost/di.hpp>

namespace di = boost::di;

struct i { virtual ~i() { } };
struct impl : i { };
struct c
{
    BOOST_DI_CTOR(c,
        boost::shared_ptr<i> p1     // i->impl, p1 == hello_world.p1
      , double p2                   // p2 = 0.0
    );
};

struct hello_world
{
    BOOST_DI_CTOR(hello_world
        , std::shared_ptr<i> p1     // i->impl
        , std::unique_ptr<i> p3     // i->impl, p3 != p1 != c.p1
        , c p4                      // by value
        , int p5                    // p5 = 0
    ) { }
};

int main() {
    using injector = di::injector<
        impl // -> di::bind<i, impl>
             // -> di::deduce<di::bind<i, impl>>
             // -> di::singleton<di::bind<i, impl>>
    >;

    auto app = injector().create<hello_world>();
}
