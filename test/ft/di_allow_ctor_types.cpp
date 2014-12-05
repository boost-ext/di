#include <type_traits>
#include <boost/di/defaults.hpp>
#include <boost/di/policies/allow_ctor_types.hpp>

namespace di = boost::di;

auto BOOST_DI_POLICIES(const di::project_scope&) noexcept {
    using namespace di::operators;
    return di::make_policies(di::allow_ctor_types(std::is_same<di::_, int>{}/* && di::is_bound{}*/));
}

#include <boost/di.hpp>

test allow_types_policy = [] {
    auto injector = di::make_injector();
    injector.create<int>();
};

