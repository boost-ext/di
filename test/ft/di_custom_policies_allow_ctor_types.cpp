#include "boost/di.hpp"
#include "boost/di/policies/allow_ctor_types.hpp"

namespace di = boost::di;

class config : public di::config {
public:
    auto policies() const noexcept {
        using namespace di::policies;
        using namespace di::policies::operators;
        return di::make_policies(allow_ctor_types(is_root{} || std::is_same<_, double>{} || is_bound<_>{}));
    }
};

struct example {
    int i = 0;
    double d = 0.0;
};

test allow_types_policy = [] {
    auto injector = di::make_injector<config>(di::bind<int>.to(42));
    injector.create<example>();
};

