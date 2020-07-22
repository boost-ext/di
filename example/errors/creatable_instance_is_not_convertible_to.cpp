//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
  struct T {
    T(int&) {}
  };
  // clang-format off
  auto injector = di::make_injector(
    di::bind<>().to(42)
  );

  injector.create<T>(); /** creatable constraint not satisfied **/
  /** scoped<instance>::is_not_convertible_to<int &>
   *  instance is not convertible to the requested type,
   *    verify binding: 'di::bind<T>.to(value)'?
   */
  // clang-format on
}
