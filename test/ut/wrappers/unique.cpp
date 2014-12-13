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

test to_interface = [] {
    //std::unique_ptr<interface> object(unique<interface*>(new implementation{}));
    //expect(dynamic_cast<implementation*>(object.get()));
};

test to_ptr = [] {
    std::unique_ptr<int> object{static_cast<int*>(unique<int*>{new int{i}})};
    expect_eq(i, *object);
};

//test to_const_ptr = [] {
    //std::unique_ptr<const int> object{unique<int*>(new int{i})};
    //expect_eq(i, *object);
//};

//test to_shared_ptr = [] {
    //std::shared_ptr<int> object((unique<int*>(i)));
    //expect_eq(i, *object);
//};

//test to_shared_ptr_other = [] {
    //boost::shared_ptr<int> object((unique<int*>(i)));
    //expect_eq(i, *object);
//};

//test to_auto_ptr = [] {
    //aux::auto_ptr<int> object((unique<int*>(i)));
    //expect_eq(i, *object);
//};

//test to_unique_ptr = [] {
    //std::unique_ptr<int> object((unique<int*>(i)));
    //expect_eq(i, *object);
//};

}}} // boost::di::wrappers

