//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di/named.hpp"

namespace boost { namespace di {

struct a { };

test named_int_value = [] {
    constexpr auto i = 42;
    using named_type = named<int, a>;
    named_type named_(i);

    expect_eq(i, named_);
};

test named_int_ref = [] {
    auto i = 42;
    using named_type = named<int&, a>;
    named_type named_(i);

    auto& ref = named_;
    ref++;

    expect_eq(i, static_cast<int&>(named_));
};

test named_const_ref = [] {
    constexpr auto i = 42;
    using named_type = named<const int&, a>;
    named_type named_(i);

    const auto& ref = static_cast<const int&>(named_);
    expect_eq(i, ref);
};

test named_ptr = [] {
    auto i = 42;
    aux::unique_ptr<int> ptr(new int{i});
    named<int*> named_(ptr.get());

    expect_eq(ptr.get(), static_cast<int*>(named_));
};

test named_const_ptr = [] {
    auto i = 42;
    aux::unique_ptr<int> ptr(new int{i});
    named<const int*> named_(ptr.get());

    expect_eq(ptr.get(), static_cast<const int*>(named_));
};

test named_rvalue_ref = [] {
    auto i = 42;
    named<int&&> named_(std::move(i));
    int&& irvalue = named_;

    expect_eq(i, irvalue);
};

test named_shared_ptr = [] {
    constexpr auto i = 42;
    using named_type = named<aux::shared_ptr<int>, a>;
    aux::shared_ptr<int> i_(new int{i});
    named_type named_(i_);

    expect_eq(i, *static_cast<aux::shared_ptr<int>>(named_));
    expect_eq(i_.get(), static_cast<aux::shared_ptr<int>>(named_).get());
};

test named_unique_ptr = [] {
    constexpr auto i = 42;
    aux::unique_ptr<int> ptr(new int{i});
    named<aux::unique_ptr<int>> named_(std::move(ptr));

    expect_eq(i, *static_cast<aux::unique_ptr<int>>(named_));
    expect_eq(ptr.get(), static_cast<aux::unique_ptr<int>>(named_).get());
};

}} // boost::di

