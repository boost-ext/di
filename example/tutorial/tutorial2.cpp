//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <string>
#include <iostream>
#include <boost/di.hpp>

namespace di = boost::di;

class app {
 public:
  app(int value, const std::string& text) : value_(value), text_(text) {}

  void run() const {
    if (value_) {
      std::cout << text_ << std::endl;
    }
  }

 private:
  int value_ = 0;
  std::string text_;
};

int main() {
  auto injector = di::make_injector(di::bind<int>().to<std::integral_constant<int, 1>>()  // compile-time value
                                    ,
                                    di::bind<std::string>().to("hello world")  // run-time value
                                    );

  injector.create<app>().run();
}
