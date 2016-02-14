//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <boost/di.hpp>

namespace di = boost::di;
//->

struct c {
  using boost_di_inject__ = di::inject<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
                                       int, int, int, int, int, int, int, int, int, int, int, int>;

  c(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
    int, int, int, int, int);
};

int main() {
  auto injector = di::make_injector();
  injector.create<c>();  // compile clean
}
