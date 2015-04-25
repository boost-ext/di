//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/aux_/utility.hpp"

namespace boost { namespace di { namespace aux {

test join_types = [] {
    expect(std::is_same<type_list<>, join_t<>>{});
    expect(std::is_same<type_list<int>, join_t<type_list<int>>>{});
    expect(std::is_same<type_list<int, double>, join_t<type_list<int>, type_list<double>>>{});
    expect(std::is_same<type_list<int, float, double>, join_t<type_list<int>, type_list<float, double>>>{});
    expect(std::is_same<type_list<float, double>, join_t<type_list<>, type_list<float, double>>>{});
};

test at = [] {
    expect(std::is_same<void, at_key_t<void, int>>{});
    expect(std::is_same<double, at_key_t<void, int, pair<int, double>>>{});
    expect(std::is_same<void, at_key_t<void, double, pair<int, double>>>{});
    expect(std::is_same<double, at_key_t<void, int, pair<int, double>, pair<float, char>>>{});
    expect(std::is_same<char, at_key_t<void, float, pair<int, double>, pair<float, char>>>{});
    expect(std::is_same<void, at_key_t<void, short, pair<int, double>, pair<float, char>>>{});
};

}}} // boost::di::aux

