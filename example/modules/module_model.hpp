//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#ifndef MODULE_MODEL_HPP
#define MODULE_MODEL_HPP
//->

#include <boost/di.hpp>

namespace di = boost::di;

class model;

di::injector<model&> module_model() noexcept;

//<-
#endif
//->
