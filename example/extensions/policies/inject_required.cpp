//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[inject_required
#include <boost/di.hpp>

namespace di = boost::di;

//<-
template<class...>
struct is_any_type : std::false_type { };

template<class T, class... Ts>
struct is_any_type<di::aux::type<di::core::any_type_fwd<T>>, Ts...> : std::true_type { };

template<class T, class... Ts>
struct is_any_type<di::aux::type<di::core::any_type_ref_fwd<T>>, Ts...> : std::true_type { };

struct inject_required {
    template<class TArg, class TDependency, class... TCtor, BOOST_DI_REQUIRES(!is_any_type<TCtor...>::value) = 0>
    std::true_type operator()(const TArg&, const TDependency&, const TCtor&...) const {
        return {};
    }

    template<class TArg, class TDependency, class... TCtor, BOOST_DI_REQUIRES(is_any_type<TCtor...>::value) = 0>
    std::false_type operator()(const TArg&, const TDependency&, const TCtor&...) const {
        using constraint_not_satisfied = typename di::policies::type<typename TArg::type>::template not_allowed_by<inject_required>;
        return constraint_not_satisfied{}.error();
    }
};
//->

/*<<create configuration with `inject_required`>>*/
class inject_required_config : public di::config {
public:
    static auto policies(...) noexcept {
        return di::make_policies(inject_required{});
    }
};

struct ctor {
    ctor(int, double) { }
};
struct example_ctor {
    example_ctor(int, ctor) { }
};

struct inject {
    BOOST_DI_INJECT(inject, int, double) { }
};
struct example_inject {
    BOOST_DI_INJECT(example_inject, int, inject) { }
};

int main() {
    /*<<make injector>>*/
    auto injector = di::make_injector<inject_required_config>();

    /*<<create type which has only injected constructors>>*/
    injector.create<example_inject>();

    /*<<create type which has arguments deduction constructors>>*/
    //injector.create<example_ctor>(); // compile error
}

//]

