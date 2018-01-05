//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <memory>

namespace di = boost::di;
//->

class my_provider : public di::config {
 public:
  static auto provider(...) noexcept { return di::providers::heap{}; }
};

int main() {
  auto injector = di::make_injector<my_provider>();
  injector.create<int>();                           // heap
  std::unique_ptr<int> i{injector.create<int*>()};  // heap
  (void)i;
}
