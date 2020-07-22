//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

//<-
// doesn't work inside polices yet / tested with gcc-5.1 and clang-3.7
static std::vector<int> v = {0};
static int i = 1;
//->

/*<<define `types dumper` directly in configuration>>*/
class types_dumper : public config {
 public:
  static auto policies(...) noexcept {
    return make_policies([](auto type) {
      using T = decltype(type);
      using arg = typename T::type;
      using name = typename T::name;
      using given = typename T::given;

      auto tab = v[i - 1];
      while (tab--) {
        std::clog << "    ";
      }

      std::clog << "(" << typeid(arg).name() << ((*(name*)(0))() ? std::string("[") + (*(name*)(0))() + std::string("]") : "")
                << " -> " << typeid(given).name() << ")" << std::endl;

      auto ctor_size = T::arity::value;
      while (ctor_size--) {
        v.insert((v.begin() + i), v[i - 1] + 1);
      }
      ++i;
    });
  }
};

}  // namespace extension
BOOST_DI_NAMESPACE_END
