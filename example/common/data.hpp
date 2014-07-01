//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_EXAMPLE_COMMON_DATA_HPP
#define BOOST_DI_EXAMPLE_COMMON_DATA_HPP

#include <memory>
#include <boost/shared_ptr.hpp>

//[data]
struct i0 { virtual ~i0() { }; };
struct c0 : i0 { };
struct c1 { c1(boost::shared_ptr<i0>, int) { } };
struct c2 { c2(int, double, char) { } };
struct c3 { c3(boost::shared_ptr<c1>, boost::shared_ptr<c2>) { } };
//]

#endif

