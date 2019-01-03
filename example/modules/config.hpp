//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
//->
#include <boost/di.hpp>

class config : public di::config {
 public:
  static auto policies(...) noexcept {
    return di::make_policies([](auto type) { std::cout << typeid(typename decltype(type)::type).name() << std::endl; });
  }
};

//<-
#endif
//->
