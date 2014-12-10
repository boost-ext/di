#define BOOST_DI_CFG global_policies
#include <boost/di.hpp>

namespace di = boost::di;

auto custom_called = false;
auto global_called = false;

struct custom_policy {
    template<typename T>
    void operator()(const T&) const noexcept {
        custom_called = true;
    }
};

struct global_policy {
    template<typename T>
    void operator()(const T&) const noexcept {
        global_called = true;
    }
};

struct custom_policies : di::config {
    auto policies() const noexcept {
        return di::make_policies(custom_policy{});
    }
};

struct global_policies : di::config {
    auto policies() const noexcept {
        return di::make_policies(global_policy{});
    }
};

test call_policies_via_global_config = [] {
    auto injector = di::make_injector();
    injector.create<int>();
    expect(global_called);
};

test call_custom_policies_although_global_config = [] {
    auto injector = di::make_injector<custom_policies>();
    injector.create<int>();
    expect(custom_called);
};

