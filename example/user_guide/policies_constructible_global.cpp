//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <type_traits>
//->
class all_must_be_bound_unless_int;
#define BOOST_DI_CFG all_must_be_bound_unless_int
#include <boost/di.hpp>

//<-
namespace di = boost::di;
//->

class all_must_be_bound_unless_int : public di::config {
 public:
  static auto policies(...) noexcept {
    using namespace di::policies;
    using namespace di::policies::operators;
    return di::make_policies(constructible(std::is_same<di::_, int>{} || is_bound<di::_>{}));
  }
};

int main() {
  assert(0 == di::make_injector().create<int>());
  // di::make_injector().create<double>(); // compile error
  assert(42.0 == make_injector(di::bind<double>().to(42.0)).create<double>());
}
