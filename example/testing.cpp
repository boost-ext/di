//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
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
    BOOST_DI_CTOR(app, std::unique_ptr<i>) { }
};

using module = di::generic_module<
    impl
>;

} // namespace

namespace ut {

struct fake : i { };

using module = di::generic_module<
    fake
>;

} // namespace ut

int main()
{
    di::injector<> injector;

    //code
    {
    injector.create<app<module>>();
    }

    //test
    {
    injector.create<app<ut::module>>();
    }

    return 0;
}

