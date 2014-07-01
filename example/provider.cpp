//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[provider
//````C++98/03/11/14```
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

/*<<`transaction` provider>>*/
struct transaction : di::provider<int>
{
    /*<<implementation of `di::provider` requirement >>*/
    virtual int get() const {
        return next();
    }

    static int& next() {
        static int i = 0;
        return ++i;
    }
};

/*<<example `usage ` class>>*/
struct usage
{
    usage(int i, std::auto_ptr<di::provider<int> > p) {
        assert(i == 0);
        assert(p->get() == 1);
        assert(p->get() == 2);
    }
};

} // namespace

int main() {
    /*<<define injector>>*/
    di::injector<transaction> injector;

    /*<<create `usage`>>*/
    injector.create<usage>();
}

//]

