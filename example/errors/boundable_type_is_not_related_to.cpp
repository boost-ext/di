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
    di::bind<int>().to<double>() /** type<double>::is_not_related_to<int> **/
  );
  // clang-format on
}
