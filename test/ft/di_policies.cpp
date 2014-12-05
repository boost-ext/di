#include <boost/di/defaults.hpp>

auto called = false;

namespace boost { namespace di {

struct policy {
    template<typename T>
    void operator()(const T&) const {
        called = true;
    }
};

template<>
struct injector_defaults<project_scope> {
	static auto policies() noexcept {
        return make_policies(policy{});
	}

	static auto provider() noexcept {
        return providers::nothrow_reduce_heap_usage{};
	}
};

}} // namespace boost::di

#include <boost/di.hpp>

namespace di = boost::di;

test call_policies = [] {
    auto injector = di::make_injector();
    injector.create<int>();
    expect(called);
};

