//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
  struct test_config /*: di::config*/ {
    static auto policies(...) { return di::make_policies(); }
  };

  // clang-format off
  di::make_injector<test_config>();
  /** config<test_config>::requires_<provider<providable_type (...)>> **/
  // clang-format on
}
