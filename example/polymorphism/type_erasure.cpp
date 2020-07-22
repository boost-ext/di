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

/*<<Type erasure>>*/
class Drawable {
  std::shared_ptr<void> ptr{};

 public:
  template <class T, class = std::enable_if_t<!std::is_convertible<T, Drawable>::value>>
  Drawable(T t) : ptr{std::make_shared<T>(t)}, draw{[this](auto&... args) { static_cast<T*>(ptr.get())->draw(args...); }} {}

  std::function<void(std::ostream&)> draw;
};

class App {
 public:
  explicit App(const Drawable drawable) : drawable{drawable} {}
  void draw(std::ostream& out) const { drawable.draw(out); }

 private:
  const Drawable drawable;
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
