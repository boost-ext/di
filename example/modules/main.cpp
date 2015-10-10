//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[main_cpp
//<-
#include <boost/di.hpp>
#include "module_view.hpp"
#include "module_model.hpp"
#include "controller.hpp"
#include "config.hpp"
//->

namespace di = boost::di;

int main() {
   auto injector = di::make_injector(
        module_view()
      , module_model()
    );

    return injector.create<controller>().run();
}

//]

