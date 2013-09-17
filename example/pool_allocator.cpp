//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct i
{
    virtual ~i() { }
};

struct c : i
{ };

struct c1
{
    BOOST_DI_CTOR(c1, boost::shared_ptr<i>) { }
};

} // namespace

template<typename T>
struct pool_allocator
{ };

int main()
{
    using module = di::module<
        di::scope<di::scopes::per_request<pool_allocator>>::bind<
            c
        >
    >;

    di::injector<module>().create<c1>();
}

