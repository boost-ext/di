//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include "logger.hpp"
#include "app.hpp"

namespace di = boost::di;

int main() {
    auto injector = di::make_injector(
        di::bind<ilogger, logger>()
    );
    injector.create<app>().start();

    return 0;
}

