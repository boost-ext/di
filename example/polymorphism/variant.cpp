//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if defined(__cpp_lib_variant)
#include <cassert>
#include <sstream>
#include <variant>

#include "common/config.hpp"

struct Square {
  void draw(std::ostream& out) const { out << "Square"; }
};

struct Circle {
  void draw(std::ostream& out) const { out << "Circle"; }
};

class Drawable : public std::variant<Square, Circle> {
 public:
  using std::variant<Square, Circle>::variant;
};

/*<<Variant>>*/
class App {
 public:
  explicit App(const Drawable drawable) : drawable{drawable} {}

  void draw(std::ostream& out) const {
    std::visit([&out](const auto& drawable) { drawable.draw(out); }, drawable);
  }

 private:
  const Drawable drawable;
};

int main() {
  std::stringstream str{};
  auto example = config().create<App>();
  example.draw(str);
  assert("Square" == str.str());
}

//<-
#else
int main() {}
#endif
//->
