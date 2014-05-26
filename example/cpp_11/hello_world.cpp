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

struct i { virtual ~i() { } };
struct impl : i { };
struct some_name { };

struct hello {
    hello(const std::shared_ptr<i>& sp, double d, std::unique_ptr<int> u)
        : sp(sp)
    {
        assert(*u==42);
        assert(dynamic_cast<impl*>(sp.get()));
        assert(d == 0.0); // default zero initialization
    }

    std::shared_ptr<i> sp;
};

struct world {
    world(hello copy
        , boost::shared_ptr<i> sp
        , int i
        , di::named<const std::string&, some_name> str
        , float& f)
        : f(f)
    {
        std::string s = str;
        assert(dynamic_cast<impl*>(sp.get()));
        assert(copy.sp.get() == sp.get());
        assert(i == 42);
        assert(s == "some_name");
    }

    float& f;
};

int main() {
    float f = 0.f;

    auto injector = di::make_injector(
        di::deduce<impl>()                                          // scope deduction -> di::shared<di::bind<i, impl>>
                                                                    // bind<i, impl> -> useful with multiple interfaces
      , di::bind_int<42>()                                          // static, compile time value
      , di::bind<std::string>::named<some_name>::to("some_name")    // external, named value
      , di::bind<float>::to(f)                                      // external reference
    );

    auto hello_world = injector.create<world>();

    hello_world.f = 42.f;
    assert(f == 42.f);

    return 0;
}

