//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <iostream>
//->
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
#if (BOOST_DI_VERSION >= 1'0'0)
  std::cout << "stable";
#else
  std::cout << "unstable";
#endif
  std::cout << " version of boost.di [" << BOOST_DI_VERSION << "]" << std::endl;
}
