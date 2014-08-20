//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONTAINS_ALL_HPP
#define BOOST_DI_CONTAINS_ALL_HPP

#include "boost/di/aux_/mpl.hpp"

namespace boost {
namespace di {

template<typename, typename>
struct contains_all;

template<typename... Ts1, typename... Ts2>
struct contains_all<type_list<Ts1...>, type_list<Ts2...>>
    : std::conditional<
		  sizeof...(Ts1) != sizeof...(Ts2)
        , std::false_type
	    , and_<bool_<contains<type_list<Ts2...>, Ts1>::value>...>
	  >::type
{ };

} // namespace di
} // namespace boost

#endif

