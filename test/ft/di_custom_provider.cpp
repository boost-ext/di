namespace boost { namespace di { namespace providers { class nothrow_heap; }}}
#define BOOST_DI_CFG_CUSTOM_PROVIDER boost::di::providers::nothrow_heap
#include <boost/di.hpp>
#include <boost/di/providers/nothrow_heap.hpp>

namespace di = boost::di;

auto called = 0;

namespace boost { namespace di {

auto custom_provider() noexcept {
    ++called;
    return providers::nothrow_heap{};
}

}} // boost::di

test call_policies = [] {
    auto injector = di::make_injector();
    injector.create<int>();
    expect_eq(1, called);
};

