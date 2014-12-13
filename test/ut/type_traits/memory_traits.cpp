//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di { namespace type_traits {

struct c { };

test traits = [] {
    expect(std::is_same<stack, memory_traits<int>::type>{});
    expect(std::is_same<stack, memory_traits<named<int>>::type>{});
    expect(std::is_same<stack, memory_traits<c>::type>{});
    expect(std::is_same<stack, memory_traits<named<c>>::type>{});
    expect(std::is_same<stack, memory_traits<const int&>::type>{});
    expect(std::is_same<stack, memory_traits<named<const int&>>::type>{});
    expect(std::is_same<heap, memory_traits<int*>::type>{});
    expect(std::is_same<heap, memory_traits<named<int*>>::type>{});
    expect(std::is_same<heap, memory_traits<const int*>::type>{});
    expect(std::is_same<heap, memory_traits<named<const int*>>::type>{});
    expect(std::is_same<heap, memory_traits<aux::shared_ptr<int>>::type>{});
    expect(std::is_same<heap, memory_traits<const aux::shared_ptr<int>&>::type>{});
    expect(std::is_same<heap, memory_traits<named<aux::shared_ptr<int>>>::type>{});
    expect(std::is_same<heap, memory_traits<const named<aux::shared_ptr<int>>&>::type>{});
    expect(std::is_same<heap, memory_traits<aux_::shared_ptr<int>>::type>{});
    expect(std::is_same<heap, memory_traits<const aux_::shared_ptr<int>&>::type>{});
    expect(std::is_same<heap, memory_traits<named<aux_::shared_ptr<int>>>::type>{});
    expect(std::is_same<heap, memory_traits<const named<aux_::shared_ptr<int>>&>::type>{});
    expect(std::is_same<heap, memory_traits<aux::weak_ptr<int>>::type>{});
    expect(std::is_same<heap, memory_traits<const aux::weak_ptr<int>&>::type>{});
    expect(std::is_same<heap, memory_traits<named<aux::weak_ptr<int>>>::type>{});
    expect(std::is_same<heap, memory_traits<const named<aux::weak_ptr<int>>&>::type>{});
    expect(std::is_same<stack, memory_traits<int&>::type>{});
    expect(std::is_same<stack, memory_traits<named<int&>>::type>{});
    expect(std::is_same<stack, memory_traits<const named<int&>&>::type>{});
    expect(std::is_same<heap, memory_traits<aux::unique_ptr<int>>::type>{});
    expect(std::is_same<heap, memory_traits<named<aux::unique_ptr<int>>>::type>{});
    expect(std::is_same<heap, memory_traits<aux::shared_ptr<int>>::type>{});
    expect(std::is_same<heap, memory_traits<named<aux::shared_ptr<int>>>::type>{});
    expect(std::is_same<heap, memory_traits<aux::weak_ptr<int>>::type>{});
    expect(std::is_same<heap, memory_traits<named<aux::weak_ptr<int>>>::type>{});
    expect(std::is_same<stack, memory_traits<int&&>::type>{});
    expect(std::is_same<stack, memory_traits<named<int&&>>::type>{});
    expect(std::is_same<stack, memory_traits<const int&&>::type>{});
    expect(std::is_same<stack, memory_traits<named<const int&&>>::type>{});
};

}}} // boost::di::type_traits


