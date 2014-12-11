//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[module1_cpp_cpp_11
//````module1.cpp```
#include "module1.hpp"

module1::injector module1::configure() const {
    return injector(
        di::bind<bool>::to(flag_)
    );
}

//]

