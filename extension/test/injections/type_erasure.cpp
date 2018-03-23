//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injections/type_erasure.hpp"

#include <cassert>
#include <sstream>
#include <string>

namespace di = boost::di;

/*<<generic interface>>*/
// clang-format off
struct Drawable : di::extension::type_erasure<Drawable> {
  using type_erasure<Drawable>::type_erasure;

  auto draw(std::ostream& out) const
    -> void REQUIRES(draw, out);
};

struct Updatable : di::extension::type_erasure<Updatable> {
  using type_erasure<Updatable>::type_erasure;

  auto update(int i)
    -> int REQUIRES_SIGN(update, int(int), i);

  auto update(const std::string& str)
    -> int REQUIRES_SIGN(update, int(const std::string&), str);
};
// clang-format on

/*<<No inheritance>>*/
struct Square {
  void draw(std::ostream& out) const { out << "Square"; }
};

/*<<No inheritance>>*/
struct Circle {
  void draw(std::ostream& out) const { out << "Circle"; }
};

/*<<No inheritance>>*/
struct Number {
  int update(int i) { return i; }
  int update(const std::string& str) { return std::stoi(str); }
};

class Example {
 public:
  Example(const Drawable drawable, Updatable updatable)
    : drawable{drawable}, updatable{updatable} {}

  void draw(std::ostream& out) const {
    drawable.draw(out);
  }

  template<class T>
  auto update(const T& value) {
    return updatable.update(value);
  }

 private:
  const Drawable drawable;
  Updatable updatable;
};

int main() {
  {
    std::stringstream str{};

    // clang-format off
    const auto injector = di::make_injector(
      di::bind<Drawable>().to<Square>(),
      di::bind<Updatable>().to<Number>()
    );
    // clang-format on

    auto example = injector.create<Example>();
    example.draw(str);
    assert("Square" == str.str());
    assert(42 == example.update(42));
    assert(42 == example.update("42"));
  }

  {
    std::stringstream str{};

    // clang-format off
    const auto injector = di::make_injector(
      di::bind<Drawable>().to<Circle>(),
      di::bind<Updatable>().to<Number>()
    );
    // clang-format on

    auto example = injector.create<Example>();
    example.draw(str);
    assert("Circle" == str.str());
    assert(42 == example.update(42));
    assert(42 == example.update("42"));
  }
}
