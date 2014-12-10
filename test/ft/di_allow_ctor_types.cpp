#include <boost/di.hpp>
#include <boost/di/policies/allow_ctor_types.hpp>

namespace di = boost::di;

struct config : di::config {
    auto policies() const noexcept {
        using namespace di::policies;
        using namespace di::policies::operators;
        return di::make_policies(allow_ctor_types(std::is_same<_, int>{} && is_bound<_>{}));
    }
};

test allow_types_policy = [] {
    auto injector = di::make_injector<config>(di::bind<int>.to(0));
    injector.create<int>();
};

