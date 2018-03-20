//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <cassert>
#include <functional>
#include <memory>
#include <sstream>
#include <type_traits>

namespace di = boost::di;

/*<<generic interface>>*/
class Drawable {
  std::shared_ptr<void> ptr{};

 public:
  template <class T, class = std::enable_if_t<!std::is_convertible<T, Drawable>::value>>
  Drawable(T t) : ptr{std::make_shared<T>(t)}, draw{[this](auto&... args) { static_cast<T*>(ptr.get())->draw(args...); }} {}

  std::function<void(std::ostream&)> draw;
};

/*<<No inheritance>>*/
struct Square {
  void draw(std::ostream& out) const { out << "Square"; }
};

/*<<No inheritance>>*/
struct Circle {
  void draw(std::ostream& out) const { out << "Circle"; }
};

int main() {
  {
    std::stringstream str{};

    // clang-format off
    const auto injector = di::make_injector(
      di::bind<Drawable>().to<Square>()
    );
    // clang-format on

    auto drawable = injector.create<Drawable>();
    drawable.draw(str);
    assert("Square" == str.str());
  }

  {
    std::stringstream str{};

    // clang-format off
    const auto injector = di::make_injector(
      di::bind<Drawable>().to<Circle>()
    );
    // clang-format on

    auto drawable = injector.create<Drawable>();
    drawable.draw(str);
    assert("Circle" == str.str());
  }
}
