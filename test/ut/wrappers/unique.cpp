//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or unique at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di/wrappers/unique.hpp"

namespace boost { namespace di { namespace wrappers {

struct interface { virtual ~interface() = default; virtual void dummy() = 0; };
struct implementation : public interface { virtual void dummy() { }; };

constexpr auto i = 42;

test to_value = [] {
    auto object = static_cast<int>(unique<int>{i});
    expect_eq(i, object);
};

test to_interface = [] {
    auto u = unique<interface*>{new implementation{}};
    auto object = std::unique_ptr<interface>{static_cast<interface*>(u)};
    expect(dynamic_cast<implementation*>(object.get()));
};

test to_ptr = [] {
    auto object = std::unique_ptr<int>{static_cast<int*>(unique<int*>{new int{i}})};
    expect_eq(i, *object);
};

test to_const_ptr = [] {
    std::unique_ptr<const int> object = unique<int*>{new int{i}};
    expect_eq(i, *object);
};

test to_copy = [] {
    auto object = static_cast<int>(unique<int*>{new int{i}});
    expect_eq(i, object);
};

test to_shared_ptr = [] {
    std::shared_ptr<int> object = unique<int*>{new int{i}};
    expect_eq(i, *object);
};

#if (__has_include(<boost/shared_ptr.hpp>))
    test to_boost_shared_ptr = [] {
        boost::shared_ptr<int> object = unique<int*>{new int{i}};
        expect_eq(i, *object);
    };
#endif

test to_unique_ptr = [] {
    std::unique_ptr<int> object = unique<int*>{new int{i}};
    expect_eq(i, *object);
};

test to_interface_from_unique_ptr = [] {
    auto u = unique<std::unique_ptr<interface>>{std::make_unique<implementation>()};
    auto object = std::unique_ptr<interface>{static_cast<interface*>(u)};
    expect(dynamic_cast<implementation*>(object.get()));
};

test to_ptr_from_unique_ptr = [] {
    auto object = std::unique_ptr<int>{static_cast<int*>(unique<std::unique_ptr<int>>{std::make_unique<int>(i)})};
    expect_eq(i, *object);
};

test to_const_ptr_from_unique_ptr = [] {
    std::unique_ptr<const int> object = unique<std::unique_ptr<int>>{std::make_unique<int>(i)};
    expect_eq(i, *object);
};

test to_copy_from_unique_ptr = [] {
    auto object = static_cast<int>(unique<std::unique_ptr<int>>{std::make_unique<int>(i)});
    expect_eq(i, object);
};

test to_shared_ptr_from_unique_ptr = [] {
    std::shared_ptr<int> object = unique<std::unique_ptr<int>>{std::make_unique<int>(i)};
    expect_eq(i, *object);
};

#if (__has_include(<boost/shared_ptr.hpp>))
    test to_boost_shared_from_unique_ptr = [] {
        boost::shared_ptr<int> object = unique<std::unique_ptr<int>>{std::make_unique<int>(i)};
        expect_eq(i, *object);
    };
#endif

test to_unique_ptr_from_unique_ptr = [] {
    std::unique_ptr<int> object = unique<std::unique_ptr<int>>{std::make_unique<int>(i)};
    expect_eq(i, *object);
};

}}} // boost::di::wrappers

