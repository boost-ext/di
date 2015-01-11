//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[model_hpp
//<-
#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include <boost/di/inject.hpp>
#include "common.hpp"
//->

namespace di = boost::di;

auto rows = []{};
auto colls = []{};

class model {
public:
    BOOST_DI_INJECT(model, (named = rows) int, (named = colls) int, std::unique_ptr<iboard>);
};

//<-
#endif
//->

//]

