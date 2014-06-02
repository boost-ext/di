//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <memory>
#include <boost/di.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/typeof/typeof.hpp>

namespace di = boost::di;

struct i { virtual ~i() { } };
struct impl : i { };
struct some_name { };

struct hello {
    hello(const boost::shared_ptr<i>& sp, double d, boost::shared_ptr<int> ap, int i)
        : sp(sp)
    {
        assert(dynamic_cast<impl*>(sp.get()));
        assert(d == 0.0); // default zero initialization
        assert(*ap == 42);
        assert(i == 42);
    }

    boost::shared_ptr<i> sp;
};

struct world {
    world(hello copy
        , boost::shared_ptr<i> sp
        , int i
        , di::named<const std::string&, some_name> s
        , float& f)
        : str(s), f(f)
    {
        assert(dynamic_cast<impl*>(sp.get()));
        assert(copy.sp.get() == sp.get());
        assert(i == 42);
        assert(str == "some_name");
    }

    world& operator=(const world&);

    std::string str;
    float& f;
};

int main() {
    float f = 0.f;

    BOOST_AUTO(injector, (di::make_injector(
        di::bind<i, impl>()                                         // scope deducted bind value
      , di::bind_int<42>()                                          // static, compile time value
      , di::bind<std::string>::named<some_name>::to("some_name")    // external, named value
      , di::bind<float>::to(boost::ref(f))                          // external reference
    )));

    world hello_world = injector.create<world>();

    hello_world.f = 42.f;
    assert(f == 42.f);

    return 0;
}

