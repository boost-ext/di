//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injections/type_erasure.hpp"

#include <cassert>
#include <sstream>

namespace di = boost::di;

/*<<generic interface>>*/
// clang-format off
struct Drawable {
  TYPE_ERASURE(Drawable) {}

  auto draw(std::ostream& out) const
    -> void REQUIRES(draw, out);
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

class Example {
 public:
  explicit Example(const Drawable drawable) : drawable{drawable} {}
  void draw(std::ostream& out) const { drawable.draw(out); }

 private:
  Drawable drawable;
};

int main() {
  {
    std::stringstream str{};

    // clang-format off
    const auto injector = di::make_injector(
      di::bind<Drawable>().to<Square>()
    );
    // clang-format on

    auto example = injector.create<Example>();
    example.draw(str);
    assert("Square" == str.str());
  }

  {
    std::stringstream str{};

    // clang-format off
    const auto injector = di::make_injector(
      di::bind<Drawable>().to<Circle>()
    );
    // clang-format on

    auto example = injector.create<Example>();
    example.draw(str);
    assert("Circle" == str.str());
  }
}
