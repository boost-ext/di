//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[module_model_cpp
//<-
#include <boost/di.hpp>
#include "module_model.hpp"
#include "model.hpp"
#include "common.hpp"
#include "config.hpp"
//->

namespace di = boost::di;

di::injector<model> module_model() noexcept {
    return di::make_injector(
        di::bind<iboard>().to<board>()
      , di::bind<int>().named(rows).to(42)
      , di::bind<int>().named(colls).to(42)
    );
}

//]

