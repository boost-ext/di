//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/int.hpp>
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
    BOOST_DI_CTOR(usage, boost::shared_ptr<di::provider<int> > p) {
        std::cout <<p->get() << std::endl; // 0
        std::cout <<p->get() << std::endl; // 1
    }
};

} // namespace

int main()
{
    di::injector<
        di::generic_module<
            transaction
        >
    > injector;

    injector.create<usage>();
}

