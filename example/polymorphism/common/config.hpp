#pragma once

#include <boost/di.hpp>

namespace di = boost::di;

class Drawable;
struct Square;

const auto config = [] {
  // clang-format off
  return di::make_injector(
    di::bind<Drawable>().to<Square>()
  );
  // clang-format on
};
