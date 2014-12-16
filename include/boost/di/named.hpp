//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_NAMED_HPP
#define BOOST_DI_NAMED_HPP

#include <memory>

namespace boost { namespace di {

struct no_name { };

template<class T, class TName = no_name>
class named : public T {
/*    named& operator=(const named&) = delete;*/

public:
    using T::T;
    using named_type = T;
    using name = TName;

   named(const T& object = {}) noexcept // non explicit
        : T(object)
    { }

    //named(const named&) = default;

    //operator const T&() const noexcept {
        //return *this;;
    //}

//private:
    /*T object_;*/
};

}} // boost::di

#endif

