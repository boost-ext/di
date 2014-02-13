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
    c(std::shared_ptr<i> p1, double p2, const std::string& s)
        : p1(p1)
    {
        assert(p2 == 0.0);
        assert(s == "text");
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
    auto injector = di::make_injector(
        di::bind_int<42>() // static
      , di::bind<std::string>::to("text") // external
      , di::bind<i, impl>() // 'impl' or 'di::deduce<di::bind<i, impl>>' or 'di::shared<di::bind<i, impl>>'
    );

    injector.create<hello_world>();
}

