//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <string>
#include <iostream>
#include <memory>
#include <cstdlib>

#include <boost/di.hpp>

#include "ilogger.hpp"
#include "logger.hpp"

namespace di = boost::di;

auto first = [] {};
auto second = [] {};

class app {
 public:
  BOOST_DI_INJECT(app, (named = first) int value1, (named = second) int value2, const std::string& text,
                  std::shared_ptr<ilogger> logger);

  void run() const;

 private:
  int value1_ = 0;
  int value2_ = 0;
  std::string text_;
  std::shared_ptr<ilogger> logger_;
};

app::app(int value1, int value2, const std::string& text, std::shared_ptr<ilogger> logger)
    : value1_(value1), value2_(value2), text_(text), logger_(logger) {}

void app::run() const {
  if (value1_ || value2_) {
    logger_->log(text_);
  }
}

auto module(const int& i) {
  return di::make_injector(di::bind<ilogger>().to<logger>(), di::bind<int>().named(first).to(i),
                           di::bind<int>().named(second).to(0), di::bind<std::string>().to("hello world"));
}

int main(int argc, char** argv) {
  auto injector = di::make_injector(module(argc > 1 ? std::atoi(argv[1]) : 0));
  injector.create<app>().run();
}
