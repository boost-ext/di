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
    BOOST_DI_INJECT_TRAITS(int, int);  // 2 parameters
    T(int, int, int, int) {}           // 4 parameters
  };

  // clang-format off
  auto injector = di::make_injector();
  injector.create<T>(); /** creatable constraint not satisfied **/
  /** type<T>::has_ambiguous_number_of_constructor_parameters::given<2>::expected<4>
   * verify BOOST_DI_INJECT_TRAITS or di::ctor_traits
   */
  // clang-format on
}
