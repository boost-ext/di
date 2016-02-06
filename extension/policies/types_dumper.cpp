//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//<-
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <typeinfo>
//->
#include <boost/di.hpp>

auto int_1 = [] { return "first int"; };
auto int_2 = [] { return "second int"; };

struct i0 {
  virtual ~i0(){};
};
struct c0 : i0 {};
struct c1 {
  c1(std::shared_ptr<i0>, int) {}
};
struct c2 {
  BOOST_DI_INJECT(c2, (named = int_1) int, (named = int_2) int, char) {}
};
struct c3 {
  c3(std::shared_ptr<c1>, std::shared_ptr<c2>) {}
};

namespace di = boost::di;

// doesn't work inside polices yet / tested with gcc-5.1 and clang-3.7
static std::vector<int> v = {0};
static int i = 1;

/*<<define `types dumper` directly in configuration>>*/
class types_dumper : public di::config {
 public:
  static auto policies(...) noexcept {
    return di::make_policies([](auto type, auto dependency, BOOST_DI_UNUSED auto... ctor) {
      using T = decltype(type);
      using arg = typename T::type;
      using name = typename T::name;
      using given = typename decltype(dependency)::given;

      auto tab = v[i - 1];
      while (tab--) {
        std::clog << "    ";
      }

      std::clog << "(" << typeid(arg).name() << ((*(name*)(0))() ? std::string("[") + (*(name*)(0))() + std::string("]") : "")
                << " -> " << typeid(given).name() << ")" << std::endl;

      auto ctor_size = sizeof...(ctor);
      while (ctor_size--) {
        v.insert((v.begin() + i), v[i - 1] + 1);
      }
      ++i;
    });
  }
};

int main() {
  /*<<define injector>>*/
  auto injector = di::make_injector<types_dumper>(di::bind<i0>().to<c0>(), di::bind<int>().named(int_1).to(42),
                                                  di::bind<int>().named(int_2).to(42));

  /*<<iterate through created objects with `types_dumper`>>*/
  injector.create<c3>();

  /*<< output [pre
      (2c3 -> 2c3)
          (St10shared_ptrI2c1E -> 2c1)
              (St10shared_ptrI2i0E -> 2c0)
              (i -> i)
          (St10shared_ptrI2c2E -> 2c2)
              (i[first int] -> i)
              (i[second int] -> i)
              (c -> c)
  ]>>*/
}

