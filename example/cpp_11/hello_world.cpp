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

struct hello {
    hello(const std::shared_ptr<int>& sp, std::unique_ptr<int> up, double d)
        : sp(sp)
    {
        assert(*sp == 0.0);
        assert(*up == 0);
        assert(d == 0.0);
    }

    std::shared_ptr<int> sp;
};

struct world {
    world(hello copy
        , boost::shared_ptr<int> sp
        , const std::string& str
        , int i)
    {
        assert(copy.sp.get() == sp.get());
        assert(str == "");
        assert(i == 0);
    }
};

struct app {
    app(hello, world) { }
};

int main() {
    di::make_injector().create<app>();
    return 0;
}

