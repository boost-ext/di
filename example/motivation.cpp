//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <string>
//->
#include <boost/di.hpp>

namespace di = boost::di;

class controller;

struct renderer {
  int device;
};

class view {
 public:
  view(std::string /*title*/, const renderer&) {}
};

class model {};
class user {};

class app {
 public:
  app(controller&, user&) {}
};

/// CREATE OBJECTS TREE
#if 1
class controller {
 public:
  controller(model&, view&) {}
};

int main() {
  /// NO DEPENDENCY INJECTION
  {
    renderer renderer_;
    view view_{"", renderer_};
    model model_;
    controller controller_{model_, view_};
    user user_;
    app app_{controller_, user_};
    (void)app_;
  }

  /// DEPENDENCY INJECTION
  {
    auto injector = di::make_injector();
    auto app_ = injector.create<app>();
    (void)app_;
  }
}
#endif

/// CHANGE CONTROLLER CONSTRUCTOR ORDER
#if 0
class controller {
 public:
  controller(view&, model&) {}
};

int main() {
  /// NO DEPENDENCY INJECTION
  {
  renderer renderer_;
  view view_{"", renderer_};
  model model_;
  //controller controller_{model_, view_};
  controller controller_{view_, model_}; /// CHANGE
  user user_;
  app app_{controller_, user_};
  (void)app_;
  }

  /// DEPENDENCY INJECTION
  {
  auto injector = di::make_injector();
  auto app_ = injector.create<app>();
  (void)app_;
  }
}
#endif

/// ADD A NEW DEPENDENCY TO CONTROLLER
#if 0
struct configuration {};
class controller {
 public:
  controller(view&, model&, configuration) {}
};

int main() {
  /// NO DEPENDENCY INJECTION
  {
  renderer renderer_;
  view view_{"", renderer_};
  model model_;
  //controller controller_{model_, view_};
  controller controller_{view_, model_, configuration{}}; /// CHANGE
  user user_;
  app app_{controller_, user_};
  (void)app_;
  }

  /// DEPENDENCY INJECTION
  {
  auto injector = di::make_injector();
  auto app_ = injector.create<app>();
  (void)app_;
  }
}
#endif
