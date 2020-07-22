//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <memory>
//->
#define BOOST_DI_CFG_DIAGNOSTICS_LEVEL 1
#include <boost/di.hpp>

namespace di = boost::di;

struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};

struct example {
  explicit example(std::unique_ptr<interface>) {}
};

int main() {
  // clang-format off
  di::make_injector().create<example>(); /** creatable constraint not satisfied **/
  /** abstract_type<interface>::is_not_bound
   *  type is not bound, did you forget to add: 
   *   'di::bind<interface>.to<implementation>()'?
   */
  // clang-format on
}
