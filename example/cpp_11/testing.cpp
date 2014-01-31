//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct i
{
    virtual ~i() { }
};

struct impl : i { };

template<typename Config>
struct app
{
    BOOST_DI_INJECT(app, std::unique_ptr<i>) { }
};

using injector_t = di::injector<impl>;

} // namespace

namespace ut {

struct fake : i { };

using injector_t = di::injector<fake>;

} // namespace ut

int main() {
    di::injector<> injector;

    //code
    {
    injector.create<app<injector_t>>();
    }

    //test
    {
    injector.create<app<ut::injector_t>>();
    }

    return 0;
}

