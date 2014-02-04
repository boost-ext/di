//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <memory>
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct i { virtual ~i() { } };
struct impl : i { };
struct c
{
    c(std::shared_ptr<i> p1, double p2)
        : p1(p1)
    {
        assert(p2 == 0.0);
    }

    std::shared_ptr<i> p1;
};

struct hello_world
{
    hello_world(std::shared_ptr<i> p1, std::unique_ptr<i> p2, c p3, int p4) {
        assert(dynamic_cast<impl*>(p1.get()));
        assert(dynamic_cast<impl*>(p2.get()));
        assert(dynamic_cast<impl*>(p3.p1.get()));
        assert(p1.get() != p2.get());
        assert(p1 == p3.p1);
        assert(p4 == 42);
    }
};

} // namespace

int main() {
    using injector_t = di::injector<
        di::bind_int<42>
      , impl // -> di::bind<i, impl>
             // -> di::deduce<di::bind<i, impl>>
             // -> di::shared<di::bind<i, impl>>
    >;

    injector_t().create<hello_world>();
}

