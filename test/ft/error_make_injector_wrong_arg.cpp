//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

test fail_when_make_injector_with_non_dependency_type = [] {
    struct neither_module_nor_injector_nor_module { };
    auto injector = di::make_injector(neither_module_nor_injector_nor_module{});
};

