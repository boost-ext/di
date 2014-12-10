#include <boost/di.hpp>

namespace di = boost::di;

auto called = false;

struct policy {
    template<typename T>
    void operator()(const T&) const noexcept {
        called = true;
    }
};

struct custom_policies : di::config {
    auto policies() const noexcept {
        return di::make_policies(policy{});
    }
};

test call_custom_policies = [] {
    called = false;
    auto injector = di::make_injector<custom_policies>();
    injector.create<int>();
    expect(called);
};

test call_global_config_policies = [] {
    called = false;
    auto injector = di::make_injector();
    injector.create<int>();
    expect(!called);
};

