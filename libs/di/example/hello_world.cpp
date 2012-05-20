//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct c0
{ };

struct c1
{
    BOOST_DI_CTOR(c1, boost::shared_ptr<c0>, int, double) { }
};

struct hello_world
{
    BOOST_DI_CTOR(hello_world, boost::shared_ptr<c1>) { }
};

} // namespace

int main()
{
    di::injector<> injector;

    {
        injector.create<hello_world>();
    }

    {
        injector.create<boost::shared_ptr<hello_world> >();
    }

    return 0;
}

