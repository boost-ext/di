//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di/aux_/utility.hpp"
#include "boost/di/wrappers/shared.hpp"
#if __has_include(<boost/shared_ptr.hpp>)
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { inline namespace v1 { namespace wrappers {

test to_shared_ptr = [] {
    auto i = std::make_shared<int>(42);
    auto object = static_cast<const std::shared_ptr<int>&>(shared<int>{i});
    expect(i == object);
};

#if __has_include(<boost/shared_ptr.hpp>)
    test to_other_shared_ptr = [] {
        auto i = std::make_shared<int>(42);
        auto object = static_cast<boost::shared_ptr<int>>(shared<int>{i});
        expect(i.get() == object.get());
    };
#endif

test to_weak_ptr = [] {
    std::weak_ptr<int> object;

    {
    auto i = std::make_shared<int>(42);
    object = static_cast<std::weak_ptr<int>>(shared<int>{i});
    auto object_ = object.lock();
    expect(i == object_);
    }

    expect(!object.lock());
};

test shared_with_ptr = [] {
    auto i = new int{42};

    struct scoped_ptr {
        aux::owner<int*> object;
        ~scoped_ptr() { delete object; }
    } ptr{i};

    shared<int*> object{ptr.object};
    expect(i == &static_cast<int&>(object));
};

}}}} // boost::di::v1::wrappers

