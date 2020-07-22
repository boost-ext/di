//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <functional>
#include <memory>
#include <sstream>
#include <type_traits>

#include "example/polymorphism/common/config.hpp"

/*<<function>>*/
class Drawable : public std::function<void(std::ostream&)> {
 public:
  using std::function<void(std::ostream&)>::function;
};

class App {
 public:
  explicit App(const Drawable drawable) : drawable{drawable} {}
  void draw(std::ostream& out) const { drawable(out); }

 private:
  const Drawable drawable;
};

struct Square {
  void operator()(std::ostream& out) const { out << "Square"; }
};

struct Circle {
  void operator()(std::ostream& out) const { out << "Circle"; }
};

int main() {
  std::stringstream str{};
  auto example = config().create<App>();
  example.draw(str);
  assert("Square" == str.str());
}
