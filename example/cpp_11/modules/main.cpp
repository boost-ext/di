//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[main_cpp_cpp_11
//````main.cpp```
#include "module1.hpp"
#include "module2.hpp"
#include "data.hpp"
#include "app.hpp"

int main(int argc, char**) {
    auto injector = boost::di::make_injector(module1(argc > 0), module2(argc));
    return injector.create<app>().run();
}

//]

