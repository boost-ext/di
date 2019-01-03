//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <cassert>
#include <memory>

namespace di = boost::di;

struct i1 {
  virtual ~i1() noexcept = default;
  virtual void dummy1() = 0;
};
struct impl1 : i1 {
  void dummy1() override {}
};
//->

int main() {
  auto injector = di::make_injector(
#if defined(__cpp_variable_templates)
      di::bind<i1>.to<impl1>()
#else
      di::bind<i1>().to<impl1>()  // no variable templates
#endif
          );

  auto object = injector.create<std::unique_ptr<i1>>();
  assert(dynamic_cast<impl1*>(object.get()));
}
