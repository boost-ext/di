//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <memory>
#include <sstream>

#include "common/config.hpp"

/*<<Dynamic polymorphism - inheritance>>*/
class Drawable {
 public:
  virtual ~Drawable() noexcept = default;
  virtual void draw(std::ostream&) const = 0;
};

class App {
 public:
  explicit App(std::unique_ptr<const Drawable> drawable) : drawable{std::move(drawable)} {}
  void draw(std::ostream& out) const { drawable->draw(out); }

 private:
  std::unique_ptr<const Drawable> drawable;
};

struct Square : Drawable {
  void draw(std::ostream& out) const override { out << "Square"; }
};

struct Circle : Drawable {
  void draw(std::ostream& out) const override { out << "Circle"; }
};

int main() {
  std::stringstream str{};
  auto example = config().create<App>();
  example.draw(str);
  assert("Square" == str.str());
}
