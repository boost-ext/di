//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
  // clang-format off
  di::make_injector(
    di::bind<int>.to(42) /** type<int>::is_bound_more_than_once **/
  , di::bind<int>.to(87) // [di::override]
  );
  // clang-format on
}
