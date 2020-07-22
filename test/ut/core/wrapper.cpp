//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/wrapper.hpp"
#include <type_traits>
#include "common/fakes/fake_wrapper.hpp"

namespace core {

test successful_wrapper = [] { expect(0 == static_cast<int>(wrapper<int, fake_wrapper>{})); };

}  // namespace core
