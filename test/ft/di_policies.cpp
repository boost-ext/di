#include <boost/di/defaults.hpp>

namespace di = boost::di;

auto called = false;

struct policy {
    template<typename T>
    void operator()(const T&) const noexcept {
        called = true;
    }
};

auto BOOST_DI_POLICIES(const di::project_scope&) noexcept {
    return di::make_policies(policy{});
}

#include <boost/di.hpp>

test call_policies = [] {
    auto injector = di::make_injector();
    injector.create<int>();
    expect(called);
};

