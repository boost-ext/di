//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <cassert>
#include <iostream>
#include <memory>
#include <typeinfo>

namespace di = boost::di;

struct i1 {
  virtual ~i1() noexcept = default;
  virtual void dummy1() = 0;
};
struct impl1 : i1 {
  void dummy1() override {}
};
//->

class print_types_info_policy : public di::config {
 public:
  static auto policies(...) noexcept {
    return di::make_policies([](auto arg) {
      using T = decltype(arg);
      using type = typename T::type;
      using name = typename T::name;
      using expected = typename T::expected;
      using given = typename T::given;
      using scope = typename T::scope;
      auto ctor_size = T::arity::value;

      std::cout << ctor_size << std::endl
                << typeid(type).name() << std::endl
                << typeid(expected).name() << std::endl
                << typeid(given).name() << std::endl
                << typeid(name).name() << std::endl
                << typeid(scope).name() << std::endl;
    });
  }
};

int main() {
  // clang-format off
  auto injector = di::make_injector<print_types_info_policy>(
    di::bind<i1>().to<impl1>()
  );
  // clang-format on
  injector.create<std::unique_ptr<i1>>();
}
