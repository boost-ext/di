//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <type_traits>
#include "boost/di/type_traits/wrapper_traits.hpp"

namespace boost { namespace di { inline namespace v1 { namespace type_traits {

struct c { };

test is_shared = [] {
    static_expect(std::is_same<wrappers::unique<int>, wrapper_traits_t<int>>::value);
    static_expect(std::is_same<wrappers::unique<c>, wrapper_traits_t<c>>::value);
    static_expect(std::is_same<wrappers::shared<int, false>, wrapper_traits_t<std::shared_ptr<int>>>::value);
    static_expect(std::is_same<wrappers::shared<c, false>, wrapper_traits_t<std::shared_ptr<c>>>::value);
};

}}}} // boost::di::v1::type_traits

