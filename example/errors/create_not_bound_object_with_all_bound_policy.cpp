//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

class must_be_bound : public di::config {
 public:
  static auto policies(...) {
    using namespace di::policies;
    return di::make_policies(constructible(is_bound<di::_>{}));
  }
};

struct example {
  example(int, double, float) {}
};

int main() {
  auto injector = di::make_injector<must_be_bound>();
  injector.create<example>();
}
