//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <string>
#include <vector>
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct renderer {
  int device;
};

class iview {
 public:
  virtual ~iview() noexcept = default;
  virtual void update() = 0;
};

class gui_view : public iview {
 public:
  gui_view(std::string title, const renderer& r) { assert(42 == r.device); }
  void update() override {}
};

class text_view : public iview {
 public:
  void update() override {}
};

class model {};

class controller {
 public:
  controller(model&, iview& v) { assert(dynamic_cast<gui_view*>(&v)); }
};

class iclient {
 public:
  virtual ~iclient() noexcept = default;
  virtual void process() = 0;
};
//->

class user : public iclient {
 public:
  void process() override{};
};

class timer : public iclient {
 public:
  void process() override{};
};

class app {
 public:
  app(controller&, std::vector<std::unique_ptr<iclient>> v) {
    assert(2 == v.size());
    assert(dynamic_cast<user*>(v[0].get()));
    assert(dynamic_cast<timer*>(v[1].get()));
  }
};

int main() {
  auto use_gui_view = true;

  // clang-format off
  auto injector = di::make_injector(
    di::bind<iview>().to([&](const auto& injector) -> iview& {
      if (use_gui_view) return (gui_view&)injector; else return (text_view&)injector;
    })
  , di::bind<int>().to(42) // renderer device
  , di::bind<iclient*[]>().to<user, timer>() // bind many clients
  );
  // clang-format on

  injector.create<app>();
}
