//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <boost/di.hpp>

namespace di = boost::di;

struct i {
  virtual ~i() noexcept = default;
  virtual void dummy() = 0;
};

struct impl : i {};

int main() { di::make_injector(di::bind<i>().to<impl>()); }
