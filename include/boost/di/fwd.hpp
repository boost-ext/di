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
class heap;
class stack_over_heap;
} // providers

struct no_name {
    const char* operator()() const noexcept { return nullptr; }
};

class config;

template<class, class...>
class injector_;

}} // boost::di

#endif

