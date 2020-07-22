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
std::vector<const std::type_info*> v = {&typeid(nullptr)};
int i = 1;
//->

/**
 * http://plantuml.sourceforge.net/objects.html
 * ./uml_dumper | java -jar plantuml.jar -p > uml_dumper.png
 */
class uml_dumper : public config {
 public:
  uml_dumper() { std::cout << "@startuml uml_dumper.png" << std::endl; }
  ~uml_dumper() { std::cout << "@enduml" << std::endl; }

  static auto policies(...) noexcept {
    /*<<define `uml dumper` directly in policies configuration>>*/
    return make_policies([&](auto type) {
      using T = decltype(type);
      using name = typename T::name;
      using given = typename T::given;

      auto root = v[i - 1];
      if (root != &typeid(nullptr)) {
        std::cout << "\"" << root->name() << "\" .. \"" << typeid(given).name()
                  << ((*(name*)(0))() ? std::string(" [") + (*(name*)(0))() + std::string("]") : "") << "\"" << std::endl;
      }

      auto ctor_size = T::arity::value;
      while (ctor_size--) {
        v.insert((v.begin() + i), &typeid(given));
      }
      ++i;
    });
  }
};

}  // namespace extension
BOOST_DI_NAMESPACE_END
