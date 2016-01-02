//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <iostream>
#include <typeinfo>
#include <cassert>
#include <boost/di.hpp>

namespace di = boost::di;

struct i1 {
  virtual ~i1() noexcept = default;
  virtual void dummy1() = 0;
};
struct impl1 : i1 {
  void dummy1() override {}
};

class print_types_info_policy : public di::config {
 public:
  static auto policies(...) noexcept {
    return di::make_policies([](auto type, auto dep, BOOST_DI_UNUSED auto... ctor) {
      using T = decltype(type);
      using arg = typename T::type;
      using arg_name = typename T::name;
      using D = decltype(dep);
      using scope = typename D::scope;
      using expected = typename D::expected;
      using given = typename D::given;
      using name = typename D::name;
      auto ctor_s = sizeof...(ctor);

      std::cout << ctor_s << std::endl
                << typeid(arg).name() << std::endl
                << typeid(arg_name).name() << std::endl
                << typeid(scope).name() << std::endl
                << typeid(expected).name() << std::endl
                << typeid(given).name() << std::endl
                << typeid(name).name() << std::endl;
    });
  }
};

int main() {
  auto injector = di::make_injector<print_types_info_policy>(di::bind<i1>().to<impl1>());

  injector.create<std::unique_ptr<i1>>();
}
