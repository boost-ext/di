//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/core/binder.hpp"
#include "boost/di/scopes/unique.hpp"

namespace boost { namespace di { namespace core {

test resolve_types_default = [] {
    struct deps { };
    auto result = binder::resolve<int>((deps*)nullptr);
    expect(std::is_same<decltype(result), dependency<scopes::deduce, int>>{});
};

test resolve_types_custom_default = [] {
    struct deps { };
    struct not_resolved { };
    auto result = binder::resolve<int, no_name, not_resolved>((deps*)nullptr);
    expect(std::is_same<decltype(result), not_resolved>{});
};

test resolve_types_found = [] {
    struct deps : dependency<scopes::unique, int> { };
    auto result = binder::resolve<int>((deps*)nullptr);
    expect(std::is_same<decltype(result), dependency<scopes::unique, int>>{});
};

test resolve_types_not_found_by_name = [] {
    struct name { };
    struct deps : dependency<scopes::unique, int, int, name> { };
    auto result = binder::resolve<int>((deps*)nullptr);
    expect(std::is_same<decltype(result), dependency<scopes::deduce, int>>{});
};

test resolve_types_found_by_name = [] {
    struct name { };
    struct deps : dependency<scopes::unique, int, int, name> { };
    auto result = binder::resolve<int, name>((deps*)nullptr);
    expect(std::is_same<decltype(result), dependency<scopes::unique, int, int, name>>{});
};

test resolve_types_found_many = [] {
    struct not_resolved { };
    struct deps
        : dependency<scopes::unique, int>
        , dependency<scopes::unique, double>
        , dependency<scopes::unique, float>
    { };

    {
    auto result = binder::resolve<float, no_name, not_resolved>((deps*)nullptr);
    expect(std::is_same<decltype(result), dependency<scopes::unique, float>>{});
    }

    {
    auto result = binder::resolve<short, no_name, not_resolved>((deps*)nullptr);
    expect(std::is_same<decltype(result), not_resolved>{});
    }
};

test resolve_types_found_priority = [] {
    struct deps
        : dependency<scopes::unique, int, int, no_name, override>
        , dependency<scopes::unique, int, int, no_name>
    { };

    auto result = binder::resolve<int>((deps*)nullptr);
    expect(std::is_same<decltype(result), dependency<scopes::unique, int, int, no_name, override>>{});
};

test resolve_types_found_priority_order = [] {
    struct deps
        : dependency<scopes::unique, int, int, no_name>
        , dependency<scopes::unique, int, int, no_name, override>
    { };

    auto result = binder::resolve<int>((deps*)nullptr);
    expect(std::is_same<decltype(result), dependency<scopes::unique, int, int, no_name, override>>{});
};

}}} // boost::di::core

