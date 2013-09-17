//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <boost/di.hpp>

namespace di  = boost::di;

namespace {

struct if0
{
    virtual ~if0() { }
    virtual void dummy() = 0;
};

struct c0if0 : if0
{
    virtual void dummy() { }
};

struct c2
{
    BOOST_DI_CTOR(c2, std::shared_ptr<if0> /*singleton*/) { }
};

struct c3
{
    BOOST_DI_CTOR(c3, std::shared_ptr<if0> /*singleton*/) { }
};

struct c4
{
    BOOST_DI_CTOR(c4, std::unique_ptr<c2> /*per_request*/, std::unique_ptr<c3> /*per_request*/) { }
};

} // namespace

int main()
{
    di::generic_module<c0if0>().create<c4>();

    return 0;
}

