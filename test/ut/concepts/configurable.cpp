//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/configurable.hpp"

namespace boost { namespace di { namespace concepts {

template<class TDeps>
decltype(configurable(std::declval<TDeps>())) configurable_v{};

test none = [] {
    class config { };
    static_assert(!configurable_v<config>, "");
};

test just_policies = [] {
    class config {
    public:
        auto policies() const noexcept { return 0; }
    };
    static_assert(!configurable_v<config>, "");
};

test just_provider = [] {
    class config {
    public:
        auto provider() const noexcept { return 0; }
    };
    static_assert(!configurable_v<config>, "");
};

test private_access = [] {
    class config {
    private:
        auto policies() const noexcept { return 0; }
        auto provider() const noexcept { return 0; }
    };
    static_assert(!configurable_v<config>, "");
};

test inheritance = [] {
    class config_ {
    public:
        auto policies() const noexcept { return 0; }
        auto provider() const noexcept { return 0; }
    };
    class config : public config_ { };
    static_assert(configurable_v<config>, "");
};

test all = [] {
    class config {
    public:
        auto policies() const noexcept { return 0; }
        auto provider() const noexcept { return 0; }
    };

    static_assert(configurable_v<config>, "");
};

}}} // boost::di::concepts

