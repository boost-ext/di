//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[module_view_cpp
//<-
#include <boost/di.hpp>
#include "module_view.hpp"
#include "common.hpp"
#include "view.hpp"
#include "config.hpp"
//->

namespace di = boost::di;

di::injector<view> module_view() noexcept {
    return di::make_injector(
        di::bind<ibutton>().to<button>()
      , di::bind<iwindow>().to<window>()
    );
}

//]

