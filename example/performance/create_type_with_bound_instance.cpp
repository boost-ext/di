//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cstdlib>
#include <string>
#include <boost/di.hpp>

namespace di = boost::di;

auto test() {
  auto injector = di::make_injector(di::bind<int>().to(42));

  return injector.create<int>();
}

int main(int, char** argv) {
  std::system(("gdb -batch -ex 'file " + std::string{argv[0]} + "' -ex 'disassemble test'").c_str());
}
