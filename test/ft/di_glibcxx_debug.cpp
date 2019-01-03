//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define _GLIBCXX_DEBUG
#include "boost/di.hpp"

#include <string>
#include <vector>

namespace di = boost::di;

int main() {
  std::vector<std::string> v{"boost", "di"};
  const auto injector = di::make_injector(di::bind<std::string[]>().to(v));
  return !injector.create<std::vector<std::string>>().empty();
}
