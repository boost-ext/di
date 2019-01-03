//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_CFG_DIAGNOSTICS_LEVEL 2
#include <boost/di.hpp>

namespace di = boost::di;

struct i {
  virtual ~i() noexcept = default;
  virtual void dummy() = 0;
};

struct impl : i {};

int main() {
  // clang-format off
  di::make_injector(
    di::bind<i>().to<impl>() /** type<impl>::is_abstract **/
    /** unimplemented pure virtual method 'dummy' in 'impl' **/
  );
  // clang-format on
}
