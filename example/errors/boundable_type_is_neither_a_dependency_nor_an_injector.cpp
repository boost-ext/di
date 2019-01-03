//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
  struct dummy {};
  // clang-format off
  di::make_injector(
    dummy{} /** type<dummy>::is_neither_a_dependency_nor_an_injector **/
  );
  // clang-format on
}
