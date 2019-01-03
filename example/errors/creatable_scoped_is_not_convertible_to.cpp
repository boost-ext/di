//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
  struct T {
    T(int*) {}
  };
  // clang-format off
  auto injector = di::make_injector(
    di::bind<int>().in(di::singleton)
  );

  injector.create<T>(); /** creatable constraint not satisfied **/
  /** scoped<singleton>::is_not_convertible_to<int *>
   *  scoped object is not convertible to the requested type,
   *    did you mistake the scope: 'di::bind<T>.in(scope)'?
   */
  // clang-format on
}
