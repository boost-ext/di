//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[provider_cpp_11
//````C++11```
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct transaction
    : di::provider<int>
{
    virtual int get() const {
        return next();
    }

    static int& next() {
        static int i = 0;
        return ++i;
    }
};

struct usage
{
    usage(std::unique_ptr<di::provider<int>> p) {
        assert(p->get() == 1);
        assert(p->get() == 2);
    }
};

} // namespace

int main() {
    using injector_t = di::injector<
        transaction
    >;

    injector_t().create<usage>();
}

//`[table
//`[[Full code example: [@example/cpp_11/provider.cpp provider.cpp]]]]
//]

