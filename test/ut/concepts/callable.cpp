//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/callable.hpp"

namespace boost { namespace di { namespace concepts {

struct non_callable_type { };

struct non_match_callable_type {
    void operator()();
};

struct non_match_callable_type_specific {
    void operator()(int);
};

struct callable_type {
    template<class T>
    void operator()(const T&);
};

struct callable_type_return {
    template<class T>
    int operator()(T) const;
};

struct callable_type_extended {
    template<class T, class TDep, class... TCtor>
    void operator()(T, TDep, TCtor...) const noexcept;
};

test is_concept_callable = [] {
    static_expect(!callable<int>());
    static_expect(!callable<non_callable_type>());
    static_expect(!callable<non_match_callable_type>());
    static_expect(!callable<non_match_callable_type_specific>());

    static_expect(callable<callable_type>());
    static_expect(callable<callable_type_return>());
    static_expect(callable<callable_type_extended>());
};

}}} // boost::di::concepts

