//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/aux_/utility.hpp"

namespace boost { namespace di { inline namespace v1 { namespace aux {

test join_types = [] {
    static_expect(std::is_same<type_list<>, join_t<>>{});
    static_expect(std::is_same<type_list<int>, join_t<type_list<int>>>{});
    static_expect(std::is_same<type_list<int, double>, join_t<type_list<int>, type_list<double>>>{});
    static_expect(std::is_same<type_list<int, float, double>, join_t<type_list<int>, type_list<float, double>>>{});
    static_expect(std::is_same<type_list<float, double>, join_t<type_list<>, type_list<float, double>>>{});
};

test is_unique_types = [] {
    static_expect(is_unique<>::value);
    static_expect(is_unique<int>::value);
    static_expect(is_unique<int, double, float>::value);
    static_expect(!is_unique<int, int>::value);
    static_expect(!is_unique<int, double, int>::value);
    static_expect(!is_unique<int, double, double, int, int>::value);
    static_expect(std::is_same<not_unique<int>, is_unique<int, int>::type>::value);
    static_expect(std::is_same<not_unique<int>, is_unique<float, int, double, int>::type>::value);
};

}}}} // boost::di::v1::aux

