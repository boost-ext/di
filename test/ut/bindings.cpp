//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/bindings.hpp"

namespace boost { namespace di { inline namespace v1 {

struct i { };
struct impl : i { };
struct name { };

test bindings_types = [] {
#if defined(__cpp_variable_templates)
    expect(std::is_base_of<core::dependency<scopes::deduce, int>, decltype(bind<int>)>{});
    expect(std::is_base_of<core::dependency<scopes::deduce, i, impl>, decltype(bind<i, impl>)>{});
#endif
    expect(std::is_base_of<core::dependency<scopes::deduce, int>, aux::remove_accessors_t<decltype(bind<int>())>>{});
    expect(std::is_base_of<core::dependency<scopes::deduce, i, impl>, aux::remove_accessors_t<decltype(bind<i, impl>())>>{});
    expect(std::is_base_of<scopes::session<name>, decltype(session(name{}))>{});
    expect(std::is_base_of<scopes::session_entry<name>, decltype(session_entry(name{}))>{});
    expect(std::is_base_of<scopes::session_exit<name>, decltype(session_exit(name{}))>{});
};

}}} // boost::di::v1

