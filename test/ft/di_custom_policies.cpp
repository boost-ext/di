#define BOOST_DI_CFG_CUSTOM_POLICIES
#include <boost/di.hpp>

namespace di = boost::di;

auto called = false;

struct policy {
    template<typename T>
    void operator()(const T&) const noexcept {
        called = true;
    }
};

namespace boost { namespace di {

template<class T, class TDependency, class TDeps>
void custom_policies(TDependency& dep, TDeps& deps) noexcept {
    call_policies<T>(dep, deps, policy{});
}

}} // boost::di

test call_policies = [] {
    auto injector = di::make_injector();
    injector.create<int>();
    expect(called);
};

