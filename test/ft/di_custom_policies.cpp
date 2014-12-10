#include <boost/di.hpp>

namespace di = boost::di;

auto called = false;

struct policy {
    template<typename T>
    void operator()(const T&) const noexcept {
        called = true;
    }
};

class custom_policies : public di::config {
public:
    auto policies() const noexcept {
        return di::make_policies(policy{});
    }
};

test call_custom_policies = [] {
    called = false;
    auto injector = di::make_injector<custom_policies>();
    injector.create<int>();
    expect_eq(1, called);
};

test call_global_config_policies = [] {
    called = false;
    auto injector = di::make_injector();
    injector.create<int>();
    expect_eq(0, called);
};

struct i { virtual ~i() = default; virtual void dummy() = 0; };
struct impl : i { void dummy() override { } };

test call_custom_policies_with_exposed_injector = [] {
    called = false;

    di::injector<i> injector = di::make_injector<custom_policies>(
        di::bind<i, impl>
    );

    auto object = injector.create<std::unique_ptr<i>>();
    expect(dynamic_cast<i*>(object.get()));
    expect_eq(1, called);
};

