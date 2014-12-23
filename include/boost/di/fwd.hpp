//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FWD_HPP
#define BOOST_DI_FWD_HPP

namespace boost { namespace di {
namespace providers {
class nothrow_heap;
class nothrow_reduce_heap_usage;
} // providers

struct no_name {
    const char* operator()() const noexcept { return "no name"; }
};

class config;
template<class...> class injector;

}} // boost::di

#endif

