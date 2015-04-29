//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[module_model_hpp
//<-
#ifndef MODULE_MODEL_HPP
#define MODULE_MODEL_HPP
//->

#include <boost/di/fwd.hpp>

namespace di = boost::di;

class model;

class module_model {
public:
    di::injector<model> configure() const noexcept;
};

//<-
#endif
//->

//]

