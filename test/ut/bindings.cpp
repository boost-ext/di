//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/bindings.hpp"

namespace boost { namespace di { inline namespace v1 {

struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct i2 { virtual ~i2() noexcept = default; virtual void dummy2() = 0; };
struct impl : i, i1, i2 { void dummy() override { } void dummy1() override { } void dummy2() override { } };

test bindings_types = [] {
#if defined(__cpp_variable_templates)
    static_expect(std::is_base_of<core::dependency<scopes::deduce, int>, decltype(bind<int>)>{});
    static_expect(std::is_base_of<core::dependency<scopes::deduce, i>, decltype(bind<i>)>{});
    static_expect(std::is_base_of<core::dependency<scopes::deduce, i, impl>, decltype(bind<i>.to<impl>())>{});
    static_expect(std::is_base_of<core::dependency<scopes::deduce, aux::type_list<i1, i2>, impl>, decltype(bind<i1, i2>.to<impl>())>{});
#endif
    static_expect(std::is_base_of<core::dependency<scopes::deduce, int>, aux::remove_specifiers_t<decltype(bind<int>())>>{});
    static_expect(std::is_base_of<core::dependency<scopes::deduce, i>, aux::remove_specifiers_t<decltype(bind<i>())>>{});
    static_expect(std::is_base_of<core::dependency<scopes::deduce, i, impl>, aux::remove_specifiers_t<decltype(bind<i>().to<impl>())>>{});
    static_expect(std::is_base_of<core::dependency<scopes::deduce, aux::type_list<i1, i2>, impl>, aux::remove_specifiers_t<decltype(bind<i1, i2>().to<impl>())>>{});
};

test any_of = [] {
    static_expect(std::is_same<int, detail::any_of_t<int>>::value);
    static_expect(std::is_same<i1, detail::any_of_t<i1>>::value);
    static_expect(std::is_same<aux::type_list<int, double>, detail::any_of_t<int, double>>::value);
    static_expect(std::is_same<aux::type_list<i1, i2>, detail::any_of_t<i1, i2>>::value);
};

}}} // boost::di::v1

