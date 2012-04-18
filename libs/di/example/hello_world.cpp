//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <boost/di.hpp>

class c1 { };
class c2 { };

namespace di = boost::di;

class Lvalue
{
public:
    BOOST_DI_CTOR(Lvalue, c1, c2)
    { }
};

class SharedPtr
{
public:
    BOOST_DI_CTOR(SharedPtr, boost::shared_ptr<c1>, boost::shared_ptr<c2>)
    { }
};

int main()
{
    di::injector<> injector;
    injector.create<Lvalue>();
    injector.create<SharedPtr>();

    return 0;
}

