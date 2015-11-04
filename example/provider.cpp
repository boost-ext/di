//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[provider
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

template<typename T>
class provider {
public:
    virtual ~provider() noexcept = default;
    virtual T get() const = 0;
};

/*<<`transaction` provider>>*/
struct transaction : provider<int> {
    /*<<implementation of `provider` requirement >>*/
    int get() const override {
        return next();
    }

    static int& next() {
        static int i = 0;
        return ++i;
    }
};

/*<<example `usage ` class>>*/
struct usage {
    usage(int i, std::unique_ptr<provider<int>> p) {
        assert(i == 0);
        assert(p->get() == 1);
        assert(p->get() == 2);
    }
};

int main() {
    /*<<define injector>>*/
    auto injector = di::make_injector(
        di::bind<provider<int>>().to<transaction>()
    );

    /*<<create `usage`>>*/
    injector.create<usage>();
}

//]

