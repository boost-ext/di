//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/type_traits/scope_traits.hpp"

namespace boost { namespace di { namespace type_traits {

struct c { };

test traits = [] {
    expect(std::is_same<scopes::unique, scope_traits<int>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<named<int>>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<c>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<named<c>>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<const int&>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<named<const int&>>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<int*>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<named<int*>>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<const int*>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<named<const int*>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<std::shared_ptr<int>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<const std::shared_ptr<int>&>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<named<std::shared_ptr<int>>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<const named<std::shared_ptr<int>>&>::type>{});
#if (__has_include(<boost/shared_ptr.hpp>))
    expect(std::is_same<scopes::singleton, scope_traits<boost::shared_ptr<int>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<const boost::shared_ptr<int>&>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<named<boost::shared_ptr<int>>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<const named<boost::shared_ptr<int>>&>::type>{});
#endif
    expect(std::is_same<scopes::singleton, scope_traits<std::weak_ptr<int>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<const std::weak_ptr<int>&>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<named<std::weak_ptr<int>>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<const named<std::weak_ptr<int>>&>::type>{});
    expect(std::is_same<scopes::external, scope_traits<int&>::type>{});
    expect(std::is_same<scopes::external, scope_traits<named<int&>>::type>{});
    expect(std::is_same<scopes::external, scope_traits<const named<int&>&>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<std::unique_ptr<int>>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<named<std::unique_ptr<int>>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<std::shared_ptr<int>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<named<std::shared_ptr<int>>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<std::weak_ptr<int>>::type>{});
    expect(std::is_same<scopes::singleton, scope_traits<named<std::weak_ptr<int>>>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<int&&>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<named<int&&>>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<const int&&>::type>{});
    expect(std::is_same<scopes::unique, scope_traits<named<const int&&>>::type>{});
};

}}} // boost::di::type_traits

