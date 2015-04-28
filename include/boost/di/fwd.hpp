//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FWD_HPP
#define BOOST_DI_FWD_HPP

namespace boost {
template<class> class shared_ptr;
namespace di {
namespace aux { struct none_t; }
namespace core {
template<class = void, class = aux::none_t, class = std::false_type>
struct any_type;
} // core
namespace providers {
class heap;
class stack_over_heap;
} // providers

struct no_name {
    const char* operator()() const noexcept { return nullptr; }
};

class config;

template<class...>
class injector;

}} // boost::di

#endif

