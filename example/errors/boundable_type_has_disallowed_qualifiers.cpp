//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
  // clang-format off
  di::make_injector(
    di::bind<int*>().to(42) /** type<int*>::has_disallowed_qualifiers **/
  );
  // clang-format on
}
