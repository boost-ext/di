#define BOOST_DI_CFG_CUSTOM_POLICIES
#include <boost/di.hpp>
#include <boost/di/policies/allow_ctor_types.hpp>

namespace di = boost::di;

namespace boost { namespace di {

template<class T, class TDependency, class TDeps>
void custom_policies(TDependency& dep, TDeps& deps) noexcept {
    using namespace policies;
    using namespace policies::operators;
    return call_policies<T>(dep, deps, allow_ctor_types(std::is_same<_, int>{} && is_bound<_>{}));
}

}} // boost::di

test allow_types_policy = [] {
    auto injector = di::make_injector(di::bind<int>.to(0));
    injector.create<int>();
};

