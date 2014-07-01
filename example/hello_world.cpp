//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[hello_world]
//````C++98/03/11/14```
//<-
#include <cassert>
#include <boost/shared_ptr.hpp>
//->
#include <boost/di.hpp>

namespace di = boost::di;

/*<define `hello` class as usual>*/
struct hello {
    hello(const boost::shared_ptr<int>& sp, std::auto_ptr<int> ap, double d)
        : sp(sp)
    {
        assert(*sp == 0.0); // value initialized with 0
        assert(*ap == 0); // value initialized with 0
        assert(d == 0.0); // value initialized with 0
    }

    boost::shared_ptr<int> sp;
};

/*<define `world` class as usual>*/
struct world {
    world(hello copy
        , boost::shared_ptr<int> sp
        , const std::string& str
        , int i)
    {
        assert(copy.sp.get() == sp.get()); // the same shared_ptr (scope deduction)
        assert(str == ""); // value initialized with empty sring
        assert(i == 0); // value initialized with 0
    }
};

/*<define `app` class as usual>*/
struct app {
    app(hello, world) { }
};

int main() {
    /*<<make injector with no configuration and create `app`>>*/
    di::make_injector().create<app>();

    return 0;
}


