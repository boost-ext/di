//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include <memory>
#include "boost/di/aux_/utility.hpp"
#include "boost/di/policies/constructible.hpp"
#include "boost/di/inject.hpp"
#include "boost/di/fwd.hpp"
#include "common/fakes/fake_policy.hpp"

namespace boost { namespace di { inline namespace v1 { namespace policies {

template<class TPolicy, class T = std::false_type>
bool constructible_test(const TPolicy&, const T& arg = {}) noexcept {
    return decltype(constructible(arg)(fake_policy<TPolicy>{}))::value;
}

template<class T = std::false_type, class T_, class TDependency, class TDeps, bool TResolve>
bool constructible_test(const fake_policy<T_, TDependency, TDeps, TResolve>& policy, const T& arg = {}) noexcept {
    return decltype(constructible(arg)(policy))::value;
}

test nothing_is_allowed = [] {
    struct c { };
    expect(!constructible_test(int{}));
    expect(!constructible_test(double{}));
    expect(!constructible_test(c{}));
};

test type_is_allowed = [] {
    auto test = [](auto type) {
        expect(constructible_test(type, std::is_same<decltype(type), _>{}));
    };

    struct c { };
    test(int{});
    test(float{});
    test(c{});
};

test type_is_not_allowed = [] {
    auto test = [](auto type) {
        expect(!constructible_test(fake_policy<void>{}, std::is_same<decltype(type), _>{}));
    };

    struct c { };
    test(int{});
    test(float{});
    test(c{});
};

test operator_not = [] {
    auto test_pass = [](auto type, auto allowed) {
        using namespace operators;
        expect(constructible_test(allowed, !std::is_same<_, decltype(type)>{}));
    };

    auto test_fail = [](auto type, auto allowed) {
        using namespace operators;
        expect(!constructible_test(allowed, !std::is_same<_, decltype(type)>{}));
    };

    struct c { };
    test_pass(double{}, int{});
    test_fail(c{}, c{});
    test_fail(int{}, int{});
    test_fail(double{}, double{});
};

test operator_or = [] {
    auto test_pass = [](auto type1, auto type2, auto allowed) {
        using namespace operators;
        expect(constructible_test(allowed, std::is_same<decltype(type1), _>{} || std::is_same<_, decltype(type2)>{}));
    };

    auto test_fail = [](auto type1, auto type2, auto allowed) {
        using namespace operators;
        expect(!constructible_test(allowed, std::is_same<decltype(type1), _>{} || std::is_same<_, decltype(type2)>{}));
    };

    test_pass(int{}, double{}, int{});
    test_pass(double{}, int{}, int{});
    test_fail(int{}, double{}, float{});
    test_fail(double{}, int{}, float{});
};

test operator_and = [] {
    auto test_pass = [](auto type, auto allowed) {
        using namespace operators;
        expect(constructible_test(allowed, std::is_integral<_>{} && std::is_same<_, decltype(type)>{}));
    };

    auto test_fail = [](auto type, auto allowed) {
        using namespace operators;
        expect(!constructible_test(allowed, std::is_integral<_>{} && std::is_same<_, decltype(type)>{}));
    };

    struct c { };
    test_pass(int{}, int{});
    test_fail(c{}, c{});
    test_fail(int{}, float{});
    test_fail(double{}, float{});
};

test is_type_bound = [] {
    expect(constructible_test(fake_policy<void, aux::none_type, aux::none_type, true>{}, is_bound<_>{}));
    expect(!constructible_test(fake_policy<void, aux::none_type, aux::none_type, false>{}, is_bound<_>{}));
};

test is_type_injected = [] {
    struct inject { BOOST_DI_INJECT(inject, int, double) { } };
    expect(!constructible_test(fake_policy<aux::none_type>{}, is_injected<_>{}));
    expect(constructible_test(fake_policy<int>{}, is_injected<_>{})); // fundamental
    expect(constructible_test(fake_policy<double>{}, is_injected<_>{})); // fundamental
    expect(constructible_test(fake_policy<float>{}, is_injected<_>{})); // fundamental
    expect(constructible_test(fake_policy<int&>{}, is_injected<_>{})); // fundamental
    expect(constructible_test(fake_policy<const int&>{}, is_injected<_>{})); // fundamental
    expect(constructible_test(fake_policy<const int*>{}, is_injected<_>{})); // fundamental
    expect(constructible_test(fake_policy<inject>{}, is_injected<_>{}));
    expect(constructible_test(fake_policy<const inject&>{}, is_injected<_>{}));
    expect(constructible_test(fake_policy<inject&>{}, is_injected<_>{}));
    expect(constructible_test(fake_policy<std::shared_ptr<inject>>{}, is_injected<_>{}));
};

test complex_opeartors = [] {
    using namespace operators;
    auto test = [](auto policy) { return constructible_test(policy, (std::is_integral<_>{} && std::is_same<_, int>{}) || is_bound<_>{}); };
    expect(!test(fake_policy<void, aux::none_type, aux::none_type, false>{}));
    expect(!test(fake_policy<double, aux::none_type, aux::none_type, false>{}));
    expect(test(fake_policy<int, aux::none_type, aux::none_type, false>{}));
    expect(test(fake_policy<int, aux::none_type, aux::none_type, true>{}));
    expect(test(fake_policy<double, aux::none_type, aux::none_type, true>{}));
};

}}}} // boost::di::v1::policies


