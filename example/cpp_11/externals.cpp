//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[externals_cpp_11
//`[h6 C++ 11]
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct i { virtual ~i() { } };
struct impl0 : i { };
struct impl1 : i { };
struct c
{
    c(std::shared_ptr<i> p, int i)
    {
        assert(dynamic_cast<impl1*>(p.get()));
        assert(i == 42);
    }
};

} // namespace

int main() {
    auto module = di::make_injector(
        di::bind<i, impl0>()
    );

    auto injector = di::make_injector(
        module
      , di::bind<int>::to(42)
      , di::bind<i>::to(std::make_shared<impl1>()) // external has priority
    );

    injector.create<c>();
}

//`full code example: [@example/cpp_11/externals.cpp externals.cpp]
//]

