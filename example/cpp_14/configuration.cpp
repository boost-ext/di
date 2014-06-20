//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[configuration_cpp_14
//`[h6 C++ 14]
//<-
//->
#include <boost/di.hpp>

namespace di = boost::di;

class module1 {
public:
    auto configure() {
        return di::make_injector(
            di::bind<i, impl>()
          , di::bind<int>::to(42)
        );
    }
};

class module2 {
public:
    auto configure() {
        return di::make_injector(
            di::bind<i2, impl2>()
        );
    }
};

int main() {
    auto injector = di::make_injector(
        std::make_tuple(module1(), module2())
    );

    injector.create<app>().start();

    return 0;
}

//`full code example: [@example/cpp_14/configuration.cpp configuration.cpp]
//]

