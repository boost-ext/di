//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <sstream>

#include "common/config.hpp"

/*<<Static polymorphism - templates>>*/
template <typename TDrawable = class Drawable>
class App {
 public:
  explicit App(const TDrawable drawable) : drawable(drawable) {}
  void draw(std::ostream& out) const { drawable.draw(out); }

 private:
  const TDrawable drawable;
};

struct Square {
  void draw(std::ostream& out) const { out << "Square"; }
};

struct Circle {
  void draw(std::ostream& out) const { out << "Circle"; }
};

int main() {
  std::stringstream str{};
  auto example = config().create<App>();
  example.draw(str);
  assert("Square" == str.str());
}
