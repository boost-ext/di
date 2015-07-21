//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/configurable.hpp"
#include "boost/di/providers/heap.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di { inline namespace v1 { namespace concepts {

test none = [] {
    class config { };
    static_expect(std::is_same<config_type<config>::is_not_configurable, configurable<config>>::value);
};

class config_just_policies {
public:
    template<class T>
    static auto policies(const T&) noexcept { return di::make_policies(); }
};

test just_policies = [] {
    static_expect(std::is_same<config_type<config_just_policies>::is_not_configurable, configurable<config_just_policies>>::value);
};

class config_just_provider {
public:
    template<class T>
    static auto provider(const T&) noexcept { return providers::heap{}; }
};

test just_provider = [] {
    static_expect(std::is_same<config_type<config_just_provider>::is_not_configurable, configurable<config_just_provider>>::value);
};

class config_private_access {
private:
    template<class T>
    static auto policies(const T&) noexcept { return di::make_policies(); }

    template<class T>
    static auto provider(const T&) noexcept { return providers::heap{}; }
};

#if !defined(BOOST_DI_MSVC)
    test private_access = [] {
        static_expect(std::is_same<config_type<config_private_access>::is_not_configurable, configurable<config_private_access>>::value);
    };
#endif

class config_inheritance_impl {
public:
    template<class T>
    static auto policies(const T&) noexcept { return di::make_policies(); }

    template<class T>
    static auto provider(const T&) noexcept { return providers::heap{}; }
};

class config_inheritance : public config_inheritance_impl { };

test inheritance = [] {
    static_expect(configurable<config_inheritance>::value);
};

class config_okay {
public:
    template<class T>
    static auto policies(const T&) noexcept { return di::make_policies(); }

    template<class T>
    static auto provider(const T&) noexcept { return providers::heap{}; }
};

test okay = [] {
    static_expect(configurable<config_okay>::value);
};

}}}} // boost::di::v1::concepts

