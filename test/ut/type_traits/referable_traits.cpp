//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/type_traits/referable_traits.hpp"
#include "common/fakes/fake_dependency.hpp"

namespace boost { namespace di { inline namespace v1 { namespace type_traits {

template<class T, class TIsReferable>
using dep = fake_dependency<T, fake_scope<>, TIsReferable>;

struct c { };

test is_referable = [] {
    static_expect(std::is_same<int, referable_traits_t<int, dep<int, std::false_type>>>::value);
    static_expect(std::is_same<int, referable_traits_t<int, dep<int, std::true_type>>>::value);
    static_expect(std::is_same<c, referable_traits_t<const c&, dep<c, std::false_type>>>::value);
    static_expect(std::is_same<const c&, referable_traits_t<const c&, dep<c, std::true_type>>>::value);
    static_expect(std::is_same<c, referable_traits_t<c&, dep<c, std::false_type>>>::value);
    static_expect(std::is_same<c&, referable_traits_t<c&, dep<c, std::true_type>>>::value);
    static_expect(std::is_same<c, referable_traits_t<c&, dep<c, std::false_type>>>::value);
    static_expect(std::is_same<c&&, referable_traits_t<c&&, dep<c, std::true_type>>>::value);
    static_expect(std::is_same<int, referable_traits_t<int&, dep<int, std::false_type>>>::value);
    static_expect(std::is_same<int&, referable_traits_t<int&, dep<int, std::true_type>>>::value);
    static_expect(std::is_same<int, referable_traits_t<const int&, dep<int, std::false_type>>>::value);
    static_expect(std::is_same<const int&, referable_traits_t<const int&, dep<int, std::true_type>>>::value);
    static_expect(std::is_same<int&&, referable_traits_t<int&&, dep<int, std::true_type>>>::value);
};

}}}} // boost::di::v1::type_traits

