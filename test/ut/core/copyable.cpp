//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/copyable.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

struct def1 { struct creator { }; };
struct def2 { struct creator { }; };
struct ctor1 { struct creator { creator(int) {} }; };
struct ctor2 { struct creator { creator(int) {} }; };

test default_constructible = [] {
    static_expect(std::is_same<aux::type_list<>, copyable_t<aux::type_list<>>>::value);
    static_expect(std::is_same<aux::type_list<>, copyable_t<aux::type_list<def1>>>::value);
    static_expect(std::is_same<aux::type_list<>, copyable_t<aux::type_list<def1, def2>>>::value);
    static_expect(std::is_same<aux::type_list<ctor1>, copyable_t<aux::type_list<def1, ctor1, def2>>>::value);
    static_expect(std::is_same<aux::type_list<ctor1, ctor2>, copyable_t<aux::type_list<def1, ctor1, def2, ctor2>>>::value);
};

}}}} // boost::di::v1::core

