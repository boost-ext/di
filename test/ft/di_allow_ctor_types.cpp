#include <type_traits>
#include <boost/di/defaults.hpp>
#include <boost/di/policies/allow_ctor_types.hpp>

namespace di = boost::di;

auto BOOST_DI_POLICIES(const di::project_scope&) noexcept {
    using namespace di::policies;
    using namespace di::policies::operators;
    return di::make_policies(
        allow_ctor_types(
            std::is_same<_, int>{} && is_bound{})
    );
}

#include <boost/di.hpp>

test allow_types_policy = [] {
    auto injector = di::make_injector(di::bind<int>.to(42));
    injector.create<int>();
};

