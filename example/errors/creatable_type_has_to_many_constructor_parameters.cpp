//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_CFG_CTOR_LIMIT_SIZE 2  // specify max number of constructor parameters
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
  struct T {
    T(int, int, int) {}  // 3 parameters
  };

  // clang-format off
  auto injector = di::make_injector();
  injector.create<T>(); /** creatable constraint not satisfied **/
  /** type<T>::has_to_many_constructor_parameters::max<2>
   *  increase BOOST_DI_CFG_CTOR_LIMIT_SIZE value or reduce number of constructor parameters
   */
  // clang-format on
}
