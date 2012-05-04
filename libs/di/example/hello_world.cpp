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

struct i { virtual ~i() { }; };
struct c : i { };

class hello_world
{
public:
    BOOST_DI_CTOR(hello_world, boost::shared_ptr<i>)
    { }
};

} // namespace

int main()
{
    typedef di::generic_module<
        di::per_request<c>
    > module;

    di::injector<module> injector;
    injector.create<hello_world>();

    return 0;
}

