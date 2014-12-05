#include <boost/di/defaults.hpp>
#include <boost/di/providers/nothrow_heap.hpp>
#include <boost/di/policies/allow_ctor_types.hpp>
#include <type_traits>

namespace boost { namespace di {

template<>
struct injector_defaults<project_scope> {
	static auto policies() noexcept {
        using namespace di::operators;
        return make_policies(allow_ctor_types(std::is_same<di::_, int>{}/* && di::is_bound{}*/));
	}

	static auto provider() noexcept {
        return providers::nothrow_heap{};
	}
};

}} // namespace boost::di

#include <boost/di.hpp>

namespace di = boost::di;

test allow_types_policy = [] {
    auto injector = di::make_injector();
    injector.create<int>();
};

