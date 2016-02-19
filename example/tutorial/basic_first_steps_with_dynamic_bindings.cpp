//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <string>
#include <cassert>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct button {
  std::string label;
  bool status;
};

class iview {
 public:
  virtual ~iview() noexcept = default;
  virtual void render() = 0;
};

class gui_view : public iview {
 public:
  gui_view(std::string title, const button& b) {
    assert("button name" == b.label);
    assert(b.status);
  }
  void render() {}
};

class text_view : public iview {
 public:
  void render() {}
};

class model {};

class controller {
 public:
  controller(model&, iview&) {}
};

class user {};

class app {
 public:
  app(controller&, user&) {}
};

int main() {
  auto use_gui_view = true;

  // clang-format off
  auto injector = di::make_injector(
    di::bind<iview>().to([&](const auto& injector) -> iview& {
      return use_gui_view ? (gui_view&)injector : (text_view&)injector;
    })
  , di::bind<std::string>().to("button name")
  , di::bind<bool>().to(true)
  );
  // clang-format on

  injector.create<app>();
}
