#define BOOST_DI_CFG global_provider
#include <boost/di.hpp>

namespace di = boost::di;

auto called = 0;

class custom_provider {
public:
    template<class, class T, class TInitialization, class TMemory, class... TArgs>
    auto get(const TInitialization&, const TMemory&, TArgs&&... args) const {
        ++called;
        return new T(std::forward<TArgs>(args)...);
    }
};

class global_provider : public di::config {
public:
    auto provider() const noexcept {
        return custom_provider{};
    }
};

test call_provider_via_global_config = [] {
    called = 0;
    auto injector = di::make_injector();
    injector.create<int>();
    expect_eq(1, called);
};

