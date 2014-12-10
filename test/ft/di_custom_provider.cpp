#include <boost/di.hpp>
#include <boost/di/providers/nothrow_heap.hpp>

namespace di = boost::di;

auto called = 0;

class config : public di::config {
public:
    auto provider() const noexcept {
        ++called;
        return di::providers::nothrow_heap{};
    }
};

test call_policies = [] {
    auto injector = di::make_injector<config>();
    injector.create<int>();
    expect_eq(1, called);
};

