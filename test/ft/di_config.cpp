//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"
#include "boost/di/providers/heap.hpp"
#include "boost/di/policies/constructible.hpp"

namespace di = boost::di;

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct impl1 : i1 { void dummy1() override { } };

static auto called = 0;

class config : public di::config {
public:
    template<class T>
    static auto policies(const T&) noexcept {
        return di::make_policies([](auto){++called;});
    }
};

test call_policy_lambda = [] {
    called = 0;
    auto injector = di::make_injector<config>();
    expect_eq(0, injector.create<int>());
    expect_eq(1, called);
};

class config_provider : public di::config {
public:
    template<class T>
    static auto provider(const T&) noexcept {
        ++called;
        return di::providers::heap{};
    }
};

test call_provider = [] {
    called = 0;
    auto injector = di::make_injector<config_provider>();
    injector.create<int>();
    expect_eq(1, called);
};

struct deleter_provider {
    static auto& called() {
        static auto i = 0;
        return i;
    }

    template<class...>
    struct is_creatable {
        static constexpr auto value = true;
    };

    template<class I, class T, class TInitialization, class TMemory, class... TArgs>
    auto get(const TInitialization& // direct/uniform
           , const TMemory& // stack/heap
           , TArgs&&... args) const {
            ++called();
        return std::unique_ptr<T, std::default_delete<I>>{
            new T(std::forward<TArgs>(args)...)
        };
    }
};

class config_deleter_provider : public di::config {
public:
    template<class T>
    static auto provider(const T&) noexcept {
        return deleter_provider{};
    }
};

test call_provider_with_deleter = [] {
    deleter_provider::called() = 0;
    auto injector = di::make_injector<config_deleter_provider>();
    injector.create<int>();
    expect_eq(1, deleter_provider::called());
};

#if !defined(BOOST_DI_MSVC)
    class config_policies : public di::config {
    public:
        template<class T>
        static auto policies(const T&) noexcept {
            using namespace di::policies;
            using namespace di::policies::operators;
            return di::make_policies(constructible(is_root{} || std::is_same<_, double>{} || is_bound<_>{}));
        }
    };

    test constructible_policy = [] {
        struct example {
            int i = 0;
            double d = 0.0;
        };

        auto injector = di::make_injector<config_policies>(di::bind<int>().to(42));
        injector.create<example>();
    };
#endif

struct policy {
    static auto& called() {
        static auto i = 0;
        return i;
    }

    template<class T>
    void operator()(const T&) const noexcept {
        ++called();
    }
};

class custom_policies : public di::config {
public:
    template<class T>
    static auto policies(const T&) noexcept {
        return di::make_policies(
            policy{}
          , [](auto) { ++policy::called(); }
          , [](BOOST_DI_UNUSED auto type, BOOST_DI_UNUSED auto dependency, BOOST_DI_UNUSED auto... ctor) { ++policy::called(); }
        );
    }
};

test call_custom_policies = [] {
    policy::called() = 0;
    auto injector = di::make_injector<custom_policies>();
    injector.create<int>();
    expect_eq(3, policy::called());
};

test call_custom_policies_with_exposed_injector = [] {
    policy::called() = 0;

    di::injector<i1> injector = di::make_injector<custom_policies>(
        di::bind<i1, impl1>()
    );

    auto object = injector.create<std::unique_ptr<i1>>();
    expect(dynamic_cast<i1*>(object.get()));
    expect_eq(3, policy::called());
};

