//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <string>
#include <iostream>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;
//<-
class ilogger {
 public:
  virtual ~ilogger() {}
  virtual void log(const std::string&) = 0;
};

class logger : public ilogger {
 public:
  void log(const std::string& text) override { std::clog << text << std::endl; }
};

class ilogic {
 public:
  virtual ~ilogic() {}
  virtual void do_it() = 0;
};

class logic : public ilogic {
 public:
  void do_it() override {}
};
//->

/*<define `hello_world` class as usual>*/
class hello_world {
 public:
  hello_world(std::shared_ptr<ilogger> logger, std::shared_ptr<ilogic> logic, bool value)
      : logger_(logger), logic_(logic), value_(value) {}

  int run() const {
    if (value_) {
      logger_->log("hello world");
      logic_->do_it();
    }
    return 0;
  }

  std::shared_ptr<ilogger> logger_;
  std::shared_ptr<ilogic> logic_;
  bool value_;
};

int main() {
  /*<<make injector configuration>>*/
  // clang-format off
  auto injector = di::make_injector(
    di::bind<ilogger>().to<logger>()
  , di::bind<ilogic>().to<logic>()
  , di::bind<bool>().to(true)
  );
  // clang-format on

  /*<<create `hello_world` and run [pre
      hello world
  ]>>*/
  return injector.create<hello_world>().run();
}
