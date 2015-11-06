//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/aux_/utility.hpp"

namespace aux {

test join_types = [] {
    static_expect(std::is_same<type_list<>, join_t<>>{});
    static_expect(std::is_same<type_list<int>, join_t<type_list<int>>>{});
    static_expect(std::is_same<type_list<int, double>, join_t<type_list<int>, type_list<double>>>{});
    static_expect(std::is_same<type_list<int, float, double>, join_t<type_list<int>, type_list<float, double>>>{});
    static_expect(std::is_same<type_list<float, double>, join_t<type_list<>, type_list<float, double>>>{});
};

test index_sequence_types = [] {
    static_expect(std::is_same<index_sequence<>, make_index_sequence<0>>::value);
    static_expect(std::is_same<index_sequence<1>, make_index_sequence<1>>::value);
    static_expect(std::is_same<index_sequence<1, 2>, make_index_sequence<2>>::value);
    static_expect(std::is_same<index_sequence<1, 2, 3>, make_index_sequence<3>>::value);
    static_expect(std::is_same<index_sequence<1, 2, 3, 4>, make_index_sequence<4>>::value);
    static_expect(std::is_same<index_sequence<1, 2, 3, 4, 5>, make_index_sequence<5>>::value);
    static_expect(std::is_same<index_sequence<1, 2, 3, 4, 5, 6>, make_index_sequence<6>>::value);
    static_expect(std::is_same<index_sequence<1, 2, 3, 4, 5, 6, 7>, make_index_sequence<7>>::value);
    static_expect(std::is_same<index_sequence<1, 2, 3, 4, 5, 6, 7, 8>, make_index_sequence<8>>::value);
    static_expect(std::is_same<index_sequence<1, 2, 3, 4, 5, 6, 7, 8, 9>, make_index_sequence<9>>::value);
    static_expect(std::is_same<index_sequence<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>, make_index_sequence<10>>::value);
};

} // aux

